#ifndef INC_3D_MOTOR_CPP_MODEL_H
#define INC_3D_MOTOR_CPP_MODEL_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include "../libs/tgaimage.h"
#include "../libs/geometry.h"

using namespace std;

struct Vector3f{
    float x =0;
    float y =0;
    float z =0;
};
struct Vector3i{
    int x = 0;
    int y = 0;
    int z = 0;
};

class Model {
protected:
    vector<Vector3f> vertices;
    vector<vector<Vector3i>> faces;
    vector<Vector3f> vt;
    vector<Vector3f> vn;
    TGAImage textureDiffuse;

public:
    Model() = default;
    explicit Model(const string& path);

    int getVerticesSize();
    Vec3f getVertexAt(int n);
    Vector3f getVertexAtVector3f(int n);

    int getFacesSize();
    vector<Vector3i> getFaceAt(int n);

    Vector3f getVtAt(int n);
    TGAColor getColorAt(float x, float y);

    Vector3f getVnAt(int n);
};


#endif //INC_3D_MOTOR_CPP_MODEL_H
