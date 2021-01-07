#include <iostream>
#include "libs/tgaimage.h"
#include <algorithm>
#include <array>

using namespace std;

constexpr int width  = 300;
constexpr int height = 300;
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
    ifstream file("obj/african_head/african_head.obj");
    vector<array<float, 3> > posVertices;
    vector<array<array<int,3>,3>> posF;

    if (file.is_open()) {
        string fline;
        float tempVertex[3];
        int tempF[3][3];

        while (getline(file, fline)) {
            if(fline.length() > 0 && !fline.rfind("v ",0)){
                fline.erase(0, fline.find(' ') + 1);
                tempVertex[0] = stof(fline.substr(0, fline.find(' ')));
                fline.erase(0, fline.find(' ') + 1);
                tempVertex[1] = stof(fline.substr(0, fline.find(' ')));
                fline.erase(0, fline.find(' ') + 1);
                tempVertex[2] = stof(fline.substr(0, fline.find(' ')));

                posVertices.push_back({tempVertex[0], tempVertex[1], tempVertex[2]});
            }
            /*
            if(fline.length() > 0 && !fline.rfind("f ",0)){
                fline.erase(0, fline.find(' ') + 1); //enleve le f et l'espace

                cout << << endl;

                //posF.push_back()
            }
            */
        }
        file.close();
    }

    TGAImage image(width, height, TGAImage::RGB);
    int posx, posy;
    for(auto vertex : posVertices){
        posx = (vertex[0]+1) *(width/2.);
        posy = (vertex[1]+1) *(height/2.);
        line(posx,posy,posx+1,posy+1, image, white);
    }
    image.write_tga_file("framebuffer.tga");

    return 0;
}

