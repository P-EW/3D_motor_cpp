#include "renderer.h"

Renderer::Renderer(Model model, TGAImage image, string save_path) {
    r_model = move(model);
    r_save_path = move(save_path);
    r_image = move(image);
    zBuffer = new float[r_image.get_height() * r_image.get_width()];
    for(int i = 0; i < r_image.get_height() * r_image.get_width(); i++){
        zBuffer[i] = INT_MIN;
    }
    modelView = lookat(camera,center,{0,1,0});
    ViewPort = viewport(r_image.get_width()/8, r_image.get_height()/8, r_image.get_width()*3/4, r_image.get_height()*3/4);
}

Matrix Renderer::viewport(int x, int y, int w, int h) {
    Matrix m = Matrix::identity(4);
    m[0][3] = x+w/2.f;
    m[1][3] = y+h/2.f;
    m[2][3] = 255/2.f;

    m[0][0] = w/2.f;
    m[1][1] = h/2.f;
    m[2][2] = 255/2.f;
    return m;
}
Matrix Renderer::v2m(Vec3f v) {
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.f;
    return m;
}
Vec3f Renderer::m2v(Matrix m) {
    return Vec3f(m[0][0]/m[3][0], m[1][0]/m[3][0], m[2][0]/m[3][0]);
}

void Renderer::render() {
    vector<Vector3i> tempFace;
    vector<Vector3f> v3f, vtList, vertices, vnList;
    Vec3f tempForcalc;
    for(int i = 0; i < r_model.getFacesSize(); i++){
        tempFace = r_model.getFaceAt(i);
        for(int j = 0; j < 3 ; j++){
            vtList.push_back(r_model.getVtAt(tempFace[j].y));
            vnList.push_back(r_model.getVnAt(tempFace[j].z));
            v3f.push_back(r_model.getVertexAt(tempFace.at(j).x));
            tempForcalc = m2v(ViewPort*Projection*modelView*v2m(vector3f2Vec3f(r_model.getVertexAt(tempFace.at(j).x))));
            vertices.push_back({tempForcalc.x,tempForcalc.y, tempForcalc.z});
        }
        // start of Gouraud shading calc
        triangle(vertices, r_image, vtList, vnList);
        // end of Gouraud shading
        vertices.clear();
        v3f.clear();
        vtList.clear();
        vnList.clear();
    }
    r_image.write_tga_file(r_save_path);
}

void Renderer::triangle(vector<Vector3f> vertices, TGAImage &image, vector<Vector3f> vtList, vector<Vector3f> vnList) {
    //detect the square occupied by the triangle
    float minX= INT_MAX , minY = INT_MAX;
    float maxX = INT_MIN, maxY = INT_MIN;
    for(int i = 0; i < vertices.size() ; i++){
        if(vertices[i].x < minX) minX = vertices[i].x;
        if(vertices[i].x > maxX) maxX = vertices[i].x;
        if(vertices[i].y < minY) minY = vertices[i].y;
        if(vertices[i].y > maxY) maxY = vertices[i].y;
    }
    if(minY < 0) minY = 0;
    if(minX < 0) minX = 0;
    if(maxY > r_image.get_height()) maxY = r_image.get_height();
    if(maxX > r_image.get_width()) maxX = r_image.get_width();

    //draw the triangle
    Vector3f pointbarycenter, shaderVector;
    float z, xi, yi, shader;
    TGAColor nmColor;
    for(int y = minY; y <= maxY; y++){
        for(int x = minX; x <= maxX; x++){
            if(pointInTriangle(Vector2i(x, y), vertices[0], vertices[1], vertices[2])){
                pointbarycenter = barycenter({x-0.f, y-0.f, 0}, vertices[0], vertices[1], vertices[2]);
                z = pointbarycenter.x * vertices[0].z + pointbarycenter.y * vertices[1].z + pointbarycenter.z * vertices[2].z;
                //Z-buffer
                if(zBuffer[x+y*image.get_height()] < z){
                    zBuffer[x+y*image.get_height()] = z;

                    // use the barycenter coordinates of the point in the first system,
                    // then calculate cartesian coordinates using vertices in the second system.
                    xi = pointbarycenter.x * (vtList[0].x) + pointbarycenter.y * (vtList[1].x) + pointbarycenter.z * (vtList[2].x);
                    yi = pointbarycenter.x * (vtList[0].y) + pointbarycenter.y * (vtList[1].y) + pointbarycenter.z * (vtList[2].y);
                    r_color = r_model.getColorAt(xi, yi);

                    nmColor = r_model.getNMColorAt(xi,yi);
                    shaderVector = {(float)nmColor.bgra[0], (float)nmColor.bgra[1], (float)nmColor.bgra[2]};
                    shader = dotProduct(normalize(shaderVector), lightDir);
                    image.set(x, y, TGAColor(shader * int(r_color.bgra[2]), shader * int(r_color.bgra[1]), shader * int(r_color.bgra[0]), int(r_color.bgra[3])));
                }
            }
        }
    }
}

bool Renderer::pointInTriangle(Vector2i p, Vector3f s0, Vector3f s1, Vector3f s2) {
    //barycentric  method
    float s = s0.y * s2.x - s0.x * s2.y + (s2.y - s0.y) * p.x + (s0.x - s2.x) * p.y;
    float t = s0.x * s1.y - s0.y * s1.x + (s0.y - s1.y) * p.x + (s1.x - s0.x) * p.y;

    if ((s < 0) != (t < 0)) return false;

    float A = -s1.y * s2.x + s0.y * (s2.x - s1.x) + s0.x * (s1.y - s2.y) + s1.x * s2.y;
    return A < 0 ? (s <= 0 && s + t >= A) : (s >= 0 && s + t <= A);
}

Vector3f Renderer::crossProduct(Vector3f vectA, Vector3f vectB) {
    return {vectA.y * vectB.z - vectA.z * vectB.y, vectA.z * vectB.x - vectA.x * vectB.z, vectA.x * vectB.y - vectA.y * vectB.x};
}

float Renderer::dotProduct(Vector3f vectA, Vector3f vectB) {
    return vectA.x * vectB.x + vectA.y * vectB.y + vectA.z * vectB.z;
}

Vector3f Renderer::barycenter(Vector3f p, Vector3f s0, Vector3f s1, Vector3f s2) {
    Vector3f v0 = {s1.x - s0.x, s1.y - s0.y , s1.z - s0.z};
    Vector3f v1 = {s2.x - s0.x, s2.y - s0.y , s2.z - s0.z};
    Vector3f v2 = {p.x - s0.x, p.y - s0.y, p.z - s0.z};
    float den = v0.x * v1.y - v1.x * v0.y;

    float a = (v2.x * v1.y - v1.x * v2.y) / den;
    float b = (v0.x * v2.y - v2.x * v0.y) / den;

    return {1.0f - a - b, a, b};
}

Vector3f Renderer::normalize(Vector3f vec) {
    float length = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    return {vec.x/length, vec.y/length, vec.z/length};
}

Matrix Renderer::lookat(Vector3f eye, Vector3f center, Vector3f up) {
    Vector3f z = normalize({eye.x-center.x,eye.y-center.y,eye.z-center.z});
    Vector3f x = normalize(crossProduct(up,z));
    Vector3f y = normalize(crossProduct(z,x));

    Matrix Minv = Matrix::identity(4);
    Minv[0][0] = x.x;
    Minv[1][0] = y.x;
    Minv[2][0] = z.x;
    Minv[0][3] = -center.x;

    Minv[0][1] = x.y;
    Minv[1][1] = y.y;
    Minv[2][1] = z.y;
    Minv[1][3] = -center.y;

    Minv[0][2] = x.z;
    Minv[1][2] = y.z;
    Minv[2][2] = z.z;
    Minv[2][3] = -center.z;

    return Minv;
}

Vec3f Renderer::vector3f2Vec3f(Vector3f v) {
    return Vec3f(v.x,v.y,v.z);
}

void Renderer::setModel(Model model) {
    r_model = std::move(model);
}

void Renderer::setCamera(Vector3f dir) {
    camera = dir;
}

void Renderer::setSavePath(string path) {
    modelView = lookat(camera,center,{0,1,0});
    ViewPort = viewport(r_image.get_width()/8, r_image.get_height()/8, r_image.get_width()*3/4, r_image.get_height()*3/4);
    zBuffer = new float[r_image.get_height() * r_image.get_width()];
    for(int i = 0; i < r_image.get_height() * r_image.get_width(); i++){
        zBuffer[i] = INT_MIN;
    }
    r_image.clear();
    r_save_path = move(path);
}
