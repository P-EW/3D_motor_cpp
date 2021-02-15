#include "libs/tgaimage.h"
#include "src/model.h"
#include "src/renderer.h"

constexpr int width  = 1024;
constexpr int height = 1024;
const TGAColor white = {255, 255, 255, 255};
const TGAColor red   = {255,   0,   0, 255};
const TGAColor green = {  0, 255,   0, 255};
const TGAColor blue  = {  0,   0, 255, 255};

int main(){
    TGAImage image(width, height, TGAImage::RGB);
    Model model("obj/african_head/african_head.obj");
    Renderer renderer(model,image,"out.tga");
    renderer.render();
    model = Model("obj/african_head/african_head_eye_inner.obj");
    renderer.setModel(model);
    renderer.render();


    //Model model("obj/diablo3_pose/diablo3_pose.obj");
    //Model model("obj/boggie/body.obj");
    //Model model("obj/floor.obj");
    //Renderer renderer(model,image,"out.tga");
    //renderer.render();

    return 0;
}

