#include "renderer.h"

Renderer::Renderer(Model model, TGAImage image, string save_path) {
    r_model = move(model);
    r_save_path = move(save_path);
    r_image = move(image);
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
        vector<int> tempFace;
        int x0, x1, y0, y1;
        for(int i = 0; i < r_model.getFacesSize(); i++){
            tempFace = r_model.getFaceAt(i);
            for(int j = 0; j < 3 ; j++){
                x0 = (r_model.getVertexAt(tempFace.at(j)).x +1) *(r_image.get_width()/2.);
                y0 = (r_model.getVertexAt(tempFace.at(j)).y +1) *(r_image.get_height()/2.);
                x1 = (r_model.getVertexAt(tempFace.at((j+1)%3)).x +1) *(r_image.get_width()/2.);
                y1 = (r_model.getVertexAt(tempFace.at((j+1)%3)).y +1) *(r_image.get_height()/2.);

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
        vector<int> tempFace;
        int x, y;
        vector<Vector2i> vertices;
        for(int i = 0; i < r_model.getFacesSize(); i++){
            tempFace = r_model.getFaceAt(i);
            for(int j = 0; j < 3 ; j++){
                x = (r_model.getVertexAt(tempFace.at(j)).x +1) *(r_image.get_width()/2.);
                y = (r_model.getVertexAt(tempFace.at(j)).y +1) *(r_image.get_height()/2.);
                vertices.emplace_back(x,y);
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
        Vector3f lightDir, vectA, vectB, temp, crossP;
        lightDir.x = 0;
        lightDir.y = 0;
        lightDir.z = 1;
        vector<int> tempFace;
        vector<Vector3f> vertices3f;
        vector<Vector2i> vertices;
        float intensity, norm, dotP;
        for(int i = 0; i < r_model.getFacesSize(); i++){
            tempFace = r_model.getFaceAt(i);
            for(int j = 0; j < 3 ; j++){
                temp.x = (r_model.getVertexAt(tempFace.at(j)).x +1) *(r_image.get_width()/2.);
                temp.y = (r_model.getVertexAt(tempFace.at(j)).y +1) *(r_image.get_height()/2.);
                temp.z = (r_model.getVertexAt(tempFace.at(j)).z +1) *(r_image.get_height()/2.);
                vertices3f.push_back(temp);
                vertices.emplace_back(temp.x,temp.y);
            }
            //construire des vecteurs a partir des points
            vectA.x = vertices3f.at(1).x - vertices3f.at(0).x;
            vectA.y = vertices3f.at(1).y - vertices3f.at(0).y;
            vectA.z = vertices3f.at(1).z - vertices3f.at(0).z;
            vectB.x = vertices3f.at(2).x - vertices3f.at(0).x;
            vectB.y = vertices3f.at(2).y - vertices3f.at(0).y;
            vectB.z = vertices3f.at(2).z - vertices3f.at(0).z;
            //calcul de la normale d'un triangle avec le cross product = normale
            crossP = crossProduct(vectB, vectA);

            //racine carrée de dot product -> norme
            norm = sqrt(crossP.x * crossP.x + crossP.y * crossP.y + crossP.z * crossP.z);

            crossP.x = crossP.x / norm;
            crossP.y = crossP.y / norm;
            crossP.z = crossP.z / norm;


            //puis dot product (produit vectoriel)
            dotP = dotProduct(crossP, lightDir);

            if(dotP < 0) {
                triangle(vertices, r_image, TGAColor(1-dotP * 255, 1-dotP * 255, 1-dotP * 255, 255));
            }

            vertices.clear();
            vertices3f.clear();
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

void Renderer::triangle(vector<Vector2i> vertices, TGAImage &image, TGAColor color) {
    //detect the square occupied by the triangle
    int minX= INT_MAX , minY = INT_MAX;
    int maxX = 0, maxY = 0;
    for(int i = 0; i < vertices.size() ; i++){
        if(vertices[i].x < minX) minX = vertices[i].x;
        if(vertices[i].x > maxX) maxX = vertices[i].x;
        if(vertices[i].y < minY) minY = vertices[i].y;
        if(vertices[i].y > maxY) maxY = vertices[i].y;
    }
    //draw the triangle
    for(int y = minY; y <= maxY; y++){
        for(int x = minX; x <= maxX; x++){
            if(pointInTriangle(Vector2i(x, y), vertices[0], vertices[1], vertices[2])){
                image.set(x, y, color);
            }
        }
    }
}

bool Renderer::pointInTriangle(Vector2i p, Vector2i s0, Vector2i s1, Vector2i s2) {
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
    Vector3f crossProduct;
    crossProduct.x = vectA.y * vectB.z - vectA.z * vectB.y;
    crossProduct.y = vectA.z * vectB.x - vectA.x * vectB.z;
    crossProduct.z = vectA.x * vectB.y - vectA.y * vectB.x;
    return crossProduct;
}

float Renderer::dotProduct(Vector3f vectA, Vector3f vectB) {
    return vectA.x * vectB.x + vectA.y * vectB.y + vectA.z * vectB.z;
}
