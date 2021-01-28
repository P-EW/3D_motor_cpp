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

void Renderer::render(const string& type) {

    if(type =="scatter"){
        int posx, posy;
        Vector3f tempVertex;
        for(int i = 0; i < r_model.getVerticesSize(); i++){
            tempVertex = r_model.getVertexAt(i);
            posx = (tempVertex.x+1) *(r_image.get_width()/2.);
            posy = (tempVertex.y+1) *(r_image.get_height()/2.);

            if(isRandomColors){
                r_image.set(posx, posy, getRandomColor());
            }
            else{
                r_image.set(posx, posy, r_color);
            }
        }
        r_image.write_tga_file(r_save_path);
    }
    else if(type =="wire"){
        vector<Vector3i> tempFace;
        int x0, x1, y0, y1;
        for(int i = 0; i < r_model.getFacesSize(); i++){
            tempFace = r_model.getFaceAt(i);
            for(int j = 0; j < 3 ; j++){
                x0 = (r_model.getVertexAt(tempFace.at(j).x).x +1) *(r_image.get_width()/2.);
                y0 = (r_model.getVertexAt(tempFace.at(j).x).y +1) *(r_image.get_height()/2.);
                x1 = (r_model.getVertexAt(tempFace.at((j+1)%3).x).x +1) *(r_image.get_width()/2.);
                y1 = (r_model.getVertexAt(tempFace.at((j+1)%3).x).y +1) *(r_image.get_height()/2.);

                if(isRandomColors){
                    line(x0,y0,x1,y1, r_image, getRandomColor());
                }
                else{
                    line(x0,y0,x1,y1, r_image, r_color);
                }
            }
        }
        r_image.write_tga_file(r_save_path);
    }
    else if(type =="triangle"){
        vector<Vector3i> tempFace;
        int x, y;
        vector<Vector3f> vertices;
        for(int i = 0; i < r_model.getFacesSize(); i++){
            tempFace = r_model.getFaceAt(i);
            for(int j = 0; j < 3 ; j++){
                vertices.push_back({(r_model.getVertexAt(tempFace.at(j).x).x +1) *(r_image.get_width()/2.f), (r_model.getVertexAt(tempFace.at(j).x).y +1) *(r_image.get_height()/2.f), r_model.getVertexAt(tempFace.at(j).x).z});
            }
            if(isRandomColors){
                triangle(vertices, r_image, getRandomColor());
            }
            else{
                triangle(vertices, r_image, r_color);
            }
            vertices.clear();
        }
        r_image.write_tga_file(r_save_path);
    }
    else if(type =="flatshading"){
        Vector3f lightDir, crossP;
        lightDir = {0,0,1};
        vector<Vector3i> tempFace;
        vector<Vector3f> v3f;
        vector<Vector3f> vertices;
        float norm, dotP;
        for(int i = 0; i < r_model.getFacesSize(); i++){
            tempFace = r_model.getFaceAt(i);
            for(int j = 0; j < 3 ; j++){
                v3f.push_back(r_model.getVertexAt(tempFace.at(j).x));
                vertices.push_back({(r_model.getVertexAt(tempFace.at(j).x).x +1) *(r_image.get_width()/2.f), (r_model.getVertexAt(tempFace.at(j).x).y +1) *(r_image.get_height()/2.f), r_model.getVertexAt(tempFace.at(j).x).z});
            }
            crossP = crossProduct({v3f.at(1).x - v3f.at(0).x, v3f.at(1).y - v3f.at(0).y, v3f.at(1).z - v3f.at(0).z}, {v3f.at(2).x - v3f.at(0).x, v3f.at(2).y - v3f.at(0).y, v3f.at(2).z - v3f.at(0).z});
            norm = sqrt(crossP.x * crossP.x + crossP.y * crossP.y + crossP.z * crossP.z);
            crossP = {crossP.x / norm, crossP.y / norm , crossP.z / norm};
            dotP = dotProduct(crossP, lightDir);
            if(dotP >= 0) {
                triangle(vertices, r_image, TGAColor(dotP * int(r_color.bgra[2]), dotP * int(r_color.bgra[1]), dotP * int(r_color.bgra[0]), int(r_color.bgra[3])));
            }
            vertices.clear();
            v3f.clear();
        }
        r_image.write_tga_file(r_save_path);
    }
}

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

void Renderer::triangle(vector<Vector3f> vertices, TGAImage &image, TGAColor color) {
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
    Vector3f temp;
    float z;
    for(int y = minY; y <= maxY; y++){
        for(int x = minX; x <= maxX; x++){
            if(pointInTriangle(Vector2i(x, y), vertices[0], vertices[1], vertices[2])){
                temp = barycenter({x,y},vertices[0],vertices[1],vertices[2]);
                z = temp.x*vertices[0].z + temp.y* vertices[1].z + temp.z * vertices[2].z;
                //Z-buffer
                if(zBuffer[x+y*image.get_height()] < z){
                    zBuffer[x+y*image.get_height()] = z;
                    image.set(x, y, color);
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

TGAColor Renderer::getRandomColor() {
    TGAColor couleur(std::rand()%255,std::rand()%255,std::rand()%255,255);
    return  couleur;
}

void Renderer::setRandomColorMode(bool status) {
    isRandomColors = status;
}

Vector3f Renderer::crossProduct(Vector3f vectA, Vector3f vectB) {
    return {vectA.y * vectB.z - vectA.z * vectB.y, vectA.z * vectB.x - vectA.x * vectB.z, vectA.x * vectB.y - vectA.y * vectB.x};
}

float Renderer::dotProduct(Vector3f vectA, Vector3f vectB) {
    return vectA.x * vectB.x + vectA.y * vectB.y + vectA.z * vectB.z;
}

void Renderer::setModel(Model model) {
    zBuffer = new float[r_image.get_height() * r_image.get_width()];
    for(int i = 0; i < r_image.get_height() * r_image.get_width(); i++){
        zBuffer[i] = INT_MIN;
    }
    r_image.clear();
    r_model = std::move(model);
}

Vector3f Renderer::barycenter(Vector2i p, Vector3f s0, Vector3f s1, Vector3f s2) {
    Vector2i v0(s1.x - s0.x, s1.y - s0.y);
    Vector2i v1(s2.x - s0.x, s2.y - s0.y);
    Vector2i v2(p.x - s0.x, p.y - s0.y);
    float den = v0.x * v1.y - v1.x * v0.y;

    return {(v2.x * v1.y - v1.x * v2.y) / den,(v0.x * v2.y - v2.x * v0.y) / den,1.0f - ((v2.x * v1.y - v1.x * v2.y) / den) - ((v0.x * v2.y - v2.x * v0.y) / den)};
}
