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
    //Model model("obj/diablo3_pose/diablo3_pose.obj");
    //Model model("obj/boggie/body.obj");


    //nuage de points
    Renderer renderer(model,image,"framebufferScatter.tga");
    renderer.render("scatter");

    renderer.setRandomColorMode(true);
    //fils de fer
    renderer.setSavePath("framebufferWire.tga");
    renderer.render("wire");

    //triangles pleins
    renderer.setSavePath("framebufferTriangle.tga");
    renderer.render("triangle");

    //flat shading
    renderer.setSavePath("framebufferFlatShading.tga");
    renderer.render("flatshading");




    // scene "2d mesh"
    //renderer.rasterize(Vector2i(20, 34),   Vector2i(744, 400), image, red);
    //renderer.rasterize(Vector2i(120, 434), Vector2i(444, 400), image, green);
    //renderer.rasterize(Vector2i(330, 463), Vector2i(594, 200), image, blue);

    return 0;
}

