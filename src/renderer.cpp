#include "renderer.h"

Renderer::Renderer(Model model, TGAImage image, string save_path) {
    r_model = move(model);
    r_save_path = move(save_path);
    r_image = move(image);
    zBuffer = new float[r_image.get_height() * r_image.get_width()];
    for(int i = 0; i < r_image.get_height() * r_image.get_width(); i++){
        zBuffer[i] = INT_MIN;
    }
}

void Renderer::render() {
    Vector3f lightDir, crossP;
    lightDir = {0,0,1};
    vector<Vector3i> tempFace;
    vector<Vector3f> v3f, vertices;
    vector<Vector3f> vtList;
    float norm, dotP;
    for(int i = 0; i < r_model.getFacesSize(); i++){
        tempFace = r_model.getFaceAt(i);
        for(int j = 0; j < 3 ; j++){
            vtList.push_back(r_model.getVtAt(tempFace[j].y));
            v3f.push_back(r_model.getVertexAt(tempFace.at(j).x));
            vertices.push_back({(r_model.getVertexAt(tempFace.at(j).x).x +1) *(r_image.get_width()/2.f), (r_model.getVertexAt(tempFace.at(j).x).y +1) *(r_image.get_height()/2.f), r_model.getVertexAt(tempFace.at(j).x).z});
        }
        // start of flat shading calc
        crossP = crossProduct({v3f.at(1).x - v3f.at(0).x, v3f.at(1).y - v3f.at(0).y, v3f.at(1).z - v3f.at(0).z}, {v3f.at(2).x - v3f.at(0).x, v3f.at(2).y - v3f.at(0).y, v3f.at(2).z - v3f.at(0).z});
        norm = sqrt(crossP.x * crossP.x + crossP.y * crossP.y + crossP.z * crossP.z);
        crossP = {crossP.x / norm, crossP.y / norm , crossP.z / norm};
        dotP = dotProduct(crossP, lightDir); //light intensity of triangle
        // end of flat shading
        if(dotP >= 0) {
            triangle(vertices, r_image, dotP, vtList);
        }
        vertices.clear();
        v3f.clear();
        vtList.clear();
    }
    r_image.write_tga_file(r_save_path);
}

void Renderer::triangle(vector<Vector3f> vertices, TGAImage &image, float dotP, vector<Vector3f> vtList) {
    //detect the square occupied by the triangle
    float minX= INT_MAX , minY = INT_MAX;
    float maxX = INT_MIN, maxY = INT_MIN;
    for(int i = 0; i < vertices.size() ; i++){
        if(vertices[i].x < minX) minX = vertices[i].x;
        if(vertices[i].x > maxX) maxX = vertices[i].x;
        if(vertices[i].y < minY) minY = vertices[i].y;
        if(vertices[i].y > maxY) maxY = vertices[i].y;
    }

    //draw the triangle
    Vector3f pointbarycenter;
    float z, xi, yi;
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
                    image.set(x, y, TGAColor(dotP * int(r_color.bgra[2]), dotP * int(r_color.bgra[1]), dotP * int(r_color.bgra[0]), int(r_color.bgra[3])));
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

//not used anymore
void Renderer::line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    bool steep = false;
    //if the line is steep, we transpose the image
    if(abs(x0-x1)<abs(y0-y1)){
        swap(x0, y0);
        swap(x1, y1);
        steep = true;
    }

    if(x0>x1){ //make it left-to-right
        swap(x0, x1);
        swap(y0, y1);
    }

    int dx = x1-x0;
    int dy = y1-y0;
    int derror3 = 2*abs(dy);
    int error3 = 0;

    int y = y0;
    for(int x = x0; x < x1 ; x++){
        if(steep){ //if transposed, de-transpose
            image.set(y, x, color);
        }
        else{
            image.set(x, y, color);
        }
        error3 += derror3;
        if(error3>dx){
            y+= (dy > 0 ? 1 : -1);
            error3 -=2*dx;
        }
    }
}
void Renderer::line(Vector2i vertex0, Vector2i vertex1, TGAImage &image, TGAColor color) {
    line(vertex0.x, vertex0.y, vertex1.x, vertex1.y, image, color);
}

TGAColor Renderer::getRandomColor() {
    TGAColor couleur(std::rand()%255,std::rand()%255,std::rand()%255,255);
    return  couleur;
}

void Renderer::setRandomColorMode(bool status) {
    isRandomColors = status;
}

void Renderer::setColor(TGAColor color) {
    r_color = color;
}

void Renderer::setSavePath(string path) {
    zBuffer = new float[r_image.get_height() * r_image.get_width()];
    for(int i = 0; i < r_image.get_height() * r_image.get_width(); i++){
        zBuffer[i] = INT_MIN;
    }
    r_image.clear();
    r_save_path = move(path);
}

void Renderer::setModel(Model model) {
    zBuffer = new float[r_image.get_height() * r_image.get_width()];
    for(int i = 0; i < r_image.get_height() * r_image.get_width(); i++){
        zBuffer[i] = INT_MIN;
    }
    r_image.clear();
    r_model = std::move(model);
}