//
// Created by julien on 12/02/19.
//

#ifndef MPROJET_MODELE_H
#define MPROJET_MODELE_H


#include "tgaimage.h"


using namespace std;

class Modele {
public:
    TGAImage diffuse;
    TGAImage nm;
    TGAImage spec;
    TGAImage glow;
    vector<vector<string> > points;
    vector<int> lignes;
    vector<vector<std::string> > textures;
    bool glowing;

    Modele();
    Modele(string filename, bool glowing);
    vector<vector<std::string> > readPoint(string filename, int type);
    vector<int> readLine(string filename);
};


#endif //MPROJET_MODELE_H
