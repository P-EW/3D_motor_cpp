#include "model.h"


Model::Model(const string& path) {
    ifstream file(path);

    if (file.is_open()) {
        string fline, tempStr, s;
        // ajout d'un sommet inutile (en pos 0) car les sommets vont de 1 à n
        vertices.push_back({0,0,0});
        vt.push_back({0,0,0});
        vector<float> vertex;
        stringstream strStrm, ss;
        vector<Vector3i> face;
        vector<string> temp;
        float tempFloat;
        while (getline(file, fline)) {
            if(fline.length() > 0 && !fline.rfind("v ",0)) {
                strStrm << fline;
                while (!strStrm.eof()) {
                    strStrm >> tempStr;
                    if (stringstream(tempStr) >> tempFloat) {
                        vertex.push_back(tempFloat);
                    }
                    tempStr = ""; //clear temp string
                }
                strStrm.clear();
                vertices.push_back({vertex[0], vertex[1], vertex[2]});
                vertex.clear();
            }
            else if(fline.length() > 0 && !fline.rfind("vt ",0)) {
                strStrm << fline;
                while (!strStrm.eof()) {
                    strStrm >> tempStr;
                    if (stringstream(tempStr) >> tempFloat) { //limitation to triangles
                        vertex.push_back(tempFloat);
                    }
                    tempStr = ""; //clear temp string
                }
                strStrm.clear();
                vt.push_back({vertex[0], vertex[1], vertex[2]});
                vertex.clear();
            }
            else if(fline.length() > 0 && !fline.rfind("f ",0)){
                strStrm << fline;
                while (!strStrm.eof()) {
                    strStrm >> tempStr;
                    if (stringstream(tempStr) >> tempFloat) {
                        ss = stringstream (tempStr);
                        s = "";
                        while (std::getline(ss, s, '/')) {
                            temp.push_back(s);
                        }
                        face.push_back({stoi(temp[0]), stoi(temp[1]), stoi(temp[2])});
                        temp.clear();
                    }
                    tempStr = ""; //clear temp string
                }
                strStrm.clear();
                faces.push_back(face);
                face.clear();
            }
        }

        file.close();
    }
}

Vector3f Model::getVertexAt(int n) {
    return vertices.at(n);
}

int Model::getVerticesSize() {
    return vertices.size();
}

vector<Vector3i> Model::getFaceAt(int n) {
    return faces.at(n);
}

int Model::getFacesSize() {
    return faces.size();
}
