#include "model.h"


Model::Model(const string& path) {
    ifstream file(path);

    if (file.is_open()) {
        string fline, tempStr;
        // ajout d'un sommet inutile (en pos 0) car les sommets vont de 1 à n
        vertices.push_back({0,0,0});
        float vertex[3];
        stringstream strStrm;
        vector<int> face;
        float tempFloat;
        int cpt;
        while (getline(file, fline)) {
            if(fline.length() > 0 && !fline.rfind("v ",0)) {
                cpt = 0;
                strStrm << fline;
                while (!strStrm.eof()) {
                    strStrm >> tempStr;
                    if (stringstream(tempStr) >> tempFloat && cpt < 3) { //limitation to triangles
                        vertex[cpt] = tempFloat;
                        cpt++;
                    }
                    tempStr = ""; //clear temp string
                }
                strStrm.clear();
                vertices.push_back({vertex[0], vertex[1], vertex[2]});
            }
            else if(fline.length() > 0 && !fline.rfind("f ",0)){
                strStrm << fline;
                while (!strStrm.eof()) {
                    strStrm >> tempStr;
                    if (stringstream(tempStr) >> tempFloat) {
                        face.push_back(tempFloat);
                    }
                    tempStr = ""; //clear temp string
                }
                strStrm.clear();
                if(face.size() == 3){ // we only do triangles here
                    faces.push_back(face);
                }
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

vector<int> Model::getFaceAt(int n) {
    return faces.at(n);
}

int Model::getFacesSize() {
    return faces.size();
}
