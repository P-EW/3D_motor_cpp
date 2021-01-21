#ifndef INC_3D_MOTOR_CPP_RENDERER_H
#define INC_3D_MOTOR_CPP_RENDERER_H

#include "model.h"
#include <cmath>
#include <algorithm>
#include "../libs/tgaimage.h"

struct Vector2i{
    Vector2i(int x_, int y_){
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
    static bool pointInTriangle(Vector2i p, Vector2i s0, Vector2i s1, Vector2i s2);
    static Vector3f crossProduct(Vector3f vectA, Vector3f vectB);
    static float dotProduct(Vector3f vectA, Vector3f vectB);

public:
    Renderer(Model model, TGAImage image, string save_path);

    void render(const string& type);
    static void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    static void line(Vector2i vertex0, Vector2i vertex1, TGAImage &image, TGAColor color);
    void setColor(TGAColor color);
    void setSavePath(string path);
    static TGAColor getRandomColor();
    void setRandomColorMode(bool status);
    static void triangle(vector<Vector2i> vertices, TGAImage &image, TGAColor color);
};


#endif //INC_3D_MOTOR_CPP_RENDERER_H
