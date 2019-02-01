//
// Created by Julien on 22/01/2019.
//

#ifndef MDRPROJET_MATRICE_H
#define MDRPROJET_MATRICE_H

#include <cstdio>
#include <vector>
#include <sstream>
#include "Vecteur.h"


using namespace std;


class Matrice {
protected:
    int size;
    float **matrice;
    int lignes;
    int colonnes;
    float c;

public:
    Matrice(int c, int l);
    int getColonnes();
    int getLignes();
    void afficher();
    Matrice multiplier(Matrice m2);
    float **getMatrice();
    static Matrice matriceId();
    float getC();
    static Matrice viewPort(float, float, float);
    static Matrice projection(Vecteur eye, Vecteur center);
    Matrice transposer();
    static Matrice lookat(Vecteur eye, Vecteur centre, Vecteur up);
};



#endif //MDRPROJET_MATRICE_H
