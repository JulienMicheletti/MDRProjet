//
// Created by julien on 12/02/19.
//

#include <vector>
#include <iostream>
#include <sstream>
#include "Modele.h"



Modele::Modele() {}

Modele::Modele(string filename, bool glowing){

    points = readPoint(filename, 0);
    lignes = readLine(filename);
    textures = readPoint(filename, 1);

    string nameDiffuse = filename + "_diffuse.tga";
    string nameNm = filename + "_nm.tga";
    string nameSpec = filename + "_spec.tga";
    string nameGlow = filename + "_glow.tga";

    diffuse.read_tga_file(nameDiffuse.c_str());
    diffuse.flip_vertically();
    nm.read_tga_file(nameNm.c_str());
    nm.flip_vertically();
    spec.read_tga_file(nameSpec.c_str());
    spec.flip_vertically();
    if (glowing) {
        glow.read_tga_file(nameGlow.c_str());
        glow.flip_vertically();
    }
    this->glowing = glowing;
};

vector<int> Modele::readLine(string filename) {
    ifstream fichier(filename.c_str(), ios::in);
    std::vector<int> tab;
    if (fichier) {
        string line;
        string esp;
        string token;
        while (getline(fichier, line)) {
            if (line[0] == 'f') {
                std::istringstream iss(line);
                while (getline(iss, esp, ' ')) {
                    if (esp[0] != 'f') {
                        std::istringstream iss2(esp);
                        for (int i = 0; i < 2; i++) {
                            getline(iss2, token, '/');
                            tab.push_back(-1 + atoi((token).c_str()));
                        }
                    }
                }
            }
        }
        fichier.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    return tab;
}

std::vector<vector<std::string> > Modele::readPoint(string filename, int type){
    ifstream fichier(filename.c_str(), ios::in);
    std::vector<std::vector<std::string> > tab;
    std::vector<std::string> tmp;
    if (fichier){
        string line;
        while(getline(fichier, line)){
            std::istringstream iss(line);
            std::string token;
            while(std::getline(iss, token, ' ')){
                tmp.push_back(token);
            }
            if (line[0] == 'v' && line[1] != 't' && type == 0){
                tab.push_back(tmp);
            }
            if (line[0] == 'v' && line[1] == 't' && type == 1){
                tab.push_back(tmp);
            }
            if (line[0] == 'v' && line[1] == 'n' && type == 2){
                tab.push_back(tmp);
            }
            tmp.clear();
        }
        fichier.close();
    }else{
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    return tab;
}
