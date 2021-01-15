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
                line(posx,posy,posx+1,posy+1, r_image, getRandomColor());
            }
            else{
                line(posx,posy,posx+1,posy+1, r_image, r_color);
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
}

void Renderer::line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
    bool steep = false;
    //if the line is steep, we transpose the image
    if(abs(x1-x0)<abs(y1-y0)){
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

void Renderer::setColor(TGAColor color) {
    r_color = color;
}

void Renderer::setSavePAth(string path) {
    r_save_path = move(path);
}

TGAColor Renderer::getRandomColor() {
    TGAColor couleur(std::rand()%255,std::rand()%255,std::rand()%255,255);
    return  couleur;
}

void Renderer::setRandomColorMode(bool status) {
    isRandomColors = status;
}
