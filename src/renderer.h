#ifndef INC_3D_MOTOR_CPP_RENDERER_H
#define INC_3D_MOTOR_CPP_RENDERER_H

#include "model.h"
#include "../libs/tgaimage.h"

struct Vertex2i{
    Vertex2i(int x_, int y_){
        x = x_;
        y = y_;
    }
    int x =0;
    int y =0;
};

class Renderer {
protected:
    Model r_model;
    string r_save_path;
    TGAImage r_image;
    TGAColor r_color = {255, 255, 255, 255}; //default is white
    bool isRandomColors = false;
    static bool pointInTriangle(Vertex2i p, Vertex2i s0, Vertex2i s1, Vertex2i s2);

public:
    Renderer(Model model, TGAImage image, string save_path);

    void render(const string& type);
    static void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    static void line(Vertex2i vertex0, Vertex2i vertex1, TGAImage &image, TGAColor color);
    void setColor(TGAColor color);
    void setSavePAth(string path);
    static TGAColor getRandomColor();
    void setRandomColorMode(bool status);
    static void triangle(vector<Vertex2i> vertices, TGAImage &image, TGAColor color);
};


#endif //INC_3D_MOTOR_CPP_RENDERER_H
