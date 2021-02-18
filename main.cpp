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
    Renderer renderer(model,image,"african.tga");
    renderer.render();
    model = Model("obj/african_head/african_head_eye_inner.obj");
    renderer.setModel(model);
    renderer.render();

    model = Model("obj/diablo3_pose/diablo3_pose.obj");
    renderer = Renderer(model,image,"diablo3.tga");
    renderer.render();

    model = Model("obj/boggie/body.obj");
    renderer = Renderer(model,image,"boggie.tga");
    renderer.render();
    model = Model("obj/boggie/head.obj");
    renderer.setModel(model);
    renderer.render();
    model = Model("obj/boggie/eyes.obj");
    renderer.setModel(model);
    renderer.render();

    /*
    Model tete("obj/african_head/african_head.obj");
    Model yeux("obj/african_head/african_head_eye_inner.obj");
    renderer = Renderer(tete,image,"out.tga");
    int IMAGE = 400;
    for(int i = 0; i < IMAGE ; i++){
        renderer.setCamera({(float)(M_PI*5)*((i-(IMAGE/2.f))/IMAGE) , 0 , 3});
        renderer.setSavePath("img/"+to_string(i)+".tga");
        renderer.setModel(tete);
        renderer.render();
        renderer.setModel(yeux);
        renderer.render();
    }
    */


    return 0;
}

