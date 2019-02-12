//
// Created by julien on 12/02/19.
//

#ifndef MPROJET_MODELE_H
#define MPROJET_MODELE_H


#include "tgaimage.h"
#include "Dessin.h"

class Modele {
public:
    TGAImage diffuse;
    TGAImage nm;
    TGAImage spec;

    Modele();
    Modele(string filename);
};


#endif //MPROJET_MODELE_H
