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
    Matrice();
    int getColonnes();
    int getLignes();
    void afficher();
    Matrice multiplier(Matrice m2);
    float **getMatrice();
    static Matrice matriceId();
    float getC();
    static Matrice viewPort(float, float, float);
    static Matrice projection(Vecteur eye, Vecteur center);
    float determinant44();
    float determinant33();
    float calculer22(float pref);
    Matrice transposer();
    Matrice comatrice();
    Matrice inverser(float);
    static Matrice matrice_M(Matrice projection, Matrice modelView);
    Matrice convertir(Vecteur);
    static Matrice matrice_MIT(Matrice matrice_M);
    Matrice concat(int, int);
    static Matrice lookat(Vecteur eye, Vecteur centre, Vecteur up);
};



#endif //MDRPROJET_MATRICE_H
