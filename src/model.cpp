#include "model.h"


Model::Model(const string& path) {
    ifstream file(path);

    if (file.is_open()) {
        string fline;
        Vector3f vertex;
        // ajout d'un sommet inutile (en pos 0) car les sommets vont de 1 à n
        vertices.push_back(vertex);

        vector<int> face;
        while (getline(file, fline)) {
            //TODO optimiser la façon de recuperer les infos ?
            if(fline.length() > 0 && !fline.rfind("v ",0)){
                fline.erase(0, fline.find(' ') + 1);
                vertex.x = stof(fline.substr(0, fline.find(' ')));
                fline.erase(0, fline.find(' ') + 1);
                vertex.y = stof(fline.substr(0, fline.find(' ')));
                fline.erase(0, fline.find(' ') + 1);
                vertex.z = stof(fline.substr(0, fline.find(' ')));

                vertices.push_back(vertex);
            }
            else if(fline.length() > 0 && !fline.rfind("f ",0)){
                fline.erase(0, fline.find(' ') + 1); //enleve le f et l'espace
                face.push_back(stoi(fline.substr(0, fline.find('/'))));

                fline.erase(0, fline.find(' ') + 1);
                face.push_back(stoi(fline.substr(0, fline.find('/'))));

                fline.erase(0, fline.find(' ') + 1);
                face.push_back(stoi(fline.substr(0, fline.find('/'))));

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

vector<int> Model::getFaceAt(int n) {
    return faces.at(n);
}

int Model::getFacesSize() {
    return faces.size();
}
