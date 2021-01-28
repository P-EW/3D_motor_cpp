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
    float* zBuffer;
    bool isRandomColors = false;
    static bool pointInTriangle(Vector2i p, Vector3f s0, Vector3f s1, Vector3f s2);
    static Vector3f crossProduct(Vector3f vectA, Vector3f vectB);
    static float dotProduct(Vector3f vectA, Vector3f vectB);
    static Vector3f barycenter(Vector2i p, Vector3f s0, Vector3f s1, Vector3f s2);

public:
    Renderer(Model model, TGAImage image, string save_path);

    void render(const string& type);
    static void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    static void line(Vector2i vertex0, Vector2i vertex1, TGAImage &image, TGAColor color);
    void setColor(TGAColor color);
    void setSavePath(string path);
    void setModel(Model model);
    static TGAColor getRandomColor();
    void setRandomColorMode(bool status);
    void triangle(vector<Vector3f> vertices, TGAImage &image, TGAColor color);
};


#endif //INC_3D_MOTOR_CPP_RENDERER_H
