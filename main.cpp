#include "libs/tgaimage.h"
#include "src/model.h"
#include "src/renderer.h"

constexpr int width  = 800;
constexpr int height = 800;
const TGAColor white = {255, 255, 255, 255};
const TGAColor red   = {255,   0,   0, 255};
const TGAColor green = {  0, 255,   0, 255};
const TGAColor blue  = {  0,   0, 255, 255};

int main(){
    TGAImage image(width, height, TGAImage::RGB);
    Model model("obj/african_head/african_head.obj");

    //nuage de points
    Renderer renderer(model,image,"framebufferScatter.tga");
    renderer.render("scatter");

    renderer.setRandomColorMode(true);
    //fils de fer
    renderer.setSavePAth("framebufferWire.tga");
    renderer.render("wire");

    return 0;
}

