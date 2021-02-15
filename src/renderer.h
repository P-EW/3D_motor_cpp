#ifndef INC_3D_MOTOR_CPP_RENDERER_H
#define INC_3D_MOTOR_CPP_RENDERER_H

#include "model.h"
#include <cmath>
#include <algorithm>
#include "../libs/tgaimage.h"
//used only for the matrix operations
#include "../libs/geometry.h"

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
    Vector3f lightDir = {0,0,1};
    Vector3f camera = {0,0,3};
    Vector3f center = {0,0,0};
    Matrix Projection = Matrix::identity(4);
    Matrix modelView;
    Matrix ViewPort;
    bool isRandomColors = false;
    static bool pointInTriangle(Vector2i p, Vector3f s0, Vector3f s1, Vector3f s2);
    static Vector3f crossProduct(Vector3f vectA, Vector3f vectB);
    static float dotProduct(Vector3f vectA, Vector3f vectB);
    static Vector3f barycenter(Vector3f p, Vector3f s0, Vector3f s1, Vector3f s2);
    Vector3f centralProjection(Vector3f point);
    Matrix v2m(Vec3f v);
    Vec3f m2v(Matrix m);
    Vec3f vector3f2Vec3f(Vector3f v);
public:
    Renderer(Model model, TGAImage image, string save_path);

    void render();
    void triangle(vector<Vector3f> vertices, TGAImage &image, float dotP, vector<Vector3f> vtList, vector<Vector3f> vnList);
    Matrix lookat(Vector3f eye, Vector3f center, Vector3f up);
    Vector3f normalize(Vector3f vec);
    static Matrix viewport(int x, int y, int w, int h);
    void setModel(Model model);
    void setCamera(Vector3f dir);
    //not used anymore
    static void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    static void line(Vector2i vertex0, Vector2i vertex1, TGAImage &image, TGAColor color);
    static TGAColor getRandomColor();
    void setRandomColorMode(bool status);
    void setColor(TGAColor color);
    void setSavePath(string path);
};


#endif //INC_3D_MOTOR_CPP_RENDERER_H
