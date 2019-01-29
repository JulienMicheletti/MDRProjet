//
// Created by Julien on 22/01/2019.
//

#ifndef MDRPROJET_MATRICE_H
#define MDRPROJET_MATRICE_H

#include <cstdio>
#include <vector>
#include <sstream>
#include "Dessin.h"
#include "Matrice.h"

class Matrice {
protected:
    int size;
    float **matrice;
    int lignes;
    int colonnes;
    pointf pointf1;
    float c;

public:
    Matrice(int c, int l);
    int getColonnes();
    int getLignes();
    void afficher();
    Matrice multiplier(Matrice m2);
    float **getMatrice();
    static Matrice matriceId();
    pointf calculerMatrice();
    float getC();
};



#endif //MDRPROJET_MATRICE_H
