#include "libs/tgaimage.h"
#include "src/model.h"

using namespace std;

constexpr int width  = 800;
constexpr int height = 800;
const TGAColor white = {255, 255, 255, 255};
const TGAColor red   = {255,   0,   0, 255};
const TGAColor green = {  0, 255,   0, 255};
const TGAColor blue  = {  0,   0, 255, 255};

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color){
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

int main(){

    Model model = Model("obj/african_head/african_head.obj");

    /*
    //nuage de points
    TGAImage image(width, height, TGAImage::RGB);
    int posx, posy;
    Vector3f tempVertex;
    for(int i = 0; i < model.getVerticesSize(); i++){
        tempVertex = model.getVertexAt(i);
        posx = (tempVertex.x+1) *(width/2.);
        posy = (tempVertex.y+1) *(height/2.);
        line(posx,posy,posx+1,posy+1, image, white);
    }
    image.write_tga_file("framebuffer.tga");
    */

    //triangles
    TGAImage image(width, height, TGAImage::RGB);
    vector<int> tempFace;
    int x0, x1, y0, y1;
    for(int i = 0; i < model.getFacesSize(); i++){
        tempFace = model.getFaceAt(i);
        for(int j = 0; j < 3 ; j++){
            x0 = (model.getVertexAt(tempFace.at(j)).x +1) *(width/2.);
            y0 = (model.getVertexAt(tempFace.at(j)).y +1) *(height/2.);
            x1 = (model.getVertexAt(tempFace.at((j+1)%3)).x +1) *(width/2.);
            y1 = (model.getVertexAt(tempFace.at((j+1)%3)).y +1) *(height/2.);
            line(x0,y0,x1,y1, image, white);
        }
    }
    image.write_tga_file("framebuffer.tga");

    return 0;
}

