#include "model.h"


Model::Model(const string& path) {
    /*
    if(!textureDiffuse.read_tga_file(path.substr(0, path.size()-4 )+"_diffuse.tga")){
        cout << "texture introuvable" << endl;
    }
    textureDiffuse.flip_vertically();
     */

    ifstream file(path);
    if (file.is_open()) {
        string fline, tempStr, s;
        // ajout d'un sommet inutile (en pos 0) car les sommets vont de 1 à n
        vertices.push_back({0,0,0});
        vt.push_back({0,0,0});
        vn.push_back({0,0,0});
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
                    if (stringstream(tempStr) >> tempFloat) {
                        vertex.push_back(tempFloat);
                    }
                    tempStr = ""; //clear temp string
                }
                strStrm.clear();
                vt.push_back({vertex[0], vertex[1], vertex[2]});
                vertex.clear();
            }
            else if(fline.length() > 0 && !fline.rfind("vn ",0)) {
                strStrm << fline;
                while (!strStrm.eof()) {
                    strStrm >> tempStr;
                    if (stringstream(tempStr) >> tempFloat) {
                        vertex.push_back(tempFloat);
                    }
                    tempStr = ""; //clear temp string
                }
                strStrm.clear();
                vn.push_back({vertex[0], vertex[1], vertex[2]});
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

TGAColor Model::getColorAt(float x, float y) {
    return (textureDiffuse.get_width() > 1) ? textureDiffuse.get(x*(textureDiffuse.get_width()), y*(textureDiffuse.get_height())) : TGAColor(255,255,255);
}

Vector3f Model::getVtAt(int n) {
    return vt.at(n);
}

Vector3f Model::getVnAt(int n) {
    return vn.at(n);
}
