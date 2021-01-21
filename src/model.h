#ifndef INC_3D_MOTOR_CPP_MODEL_H
#define INC_3D_MOTOR_CPP_MODEL_H

#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct Vector3f{
    float x =0;
    float y =0;
    float z =0;
};

class Model {
protected:
    vector<Vector3f> vertices;
    vector<vector<int>> faces;

public:
    Model() = default;
    explicit Model(const string& path);

    int getVerticesSize();
    Vector3f getVertexAt(int n);

    int getFacesSize();
    vector<int> getFaceAt(int n);
};


#endif //INC_3D_MOTOR_CPP_MODEL_H
