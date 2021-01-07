#ifndef INC_3D_MOTOR_CPP_RENDERER_H
#define INC_3D_MOTOR_CPP_RENDERER_H

#include "model.h"
#include "../libs/tgaimage.h"


class Renderer {
protected:
    Model r_model;
    string r_save_path;
    TGAImage r_image;
    TGAColor r_color = {255, 255, 255, 255}; //default is white

public:
    Renderer(Model model, TGAImage image, string save_path);

    void render(const string& type);
    static void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    void setColor(TGAColor color);
    void setSavePAth(string path);
};


#endif //INC_3D_MOTOR_CPP_RENDERER_H
