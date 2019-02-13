//
// Created by Julien on 16/01/2019.
//

#ifndef MDRPROJET_DESSIN_H
#define MDRPROJET_DESSIN_H


#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include "tgaimage.h"
#include "Vecteur.h"
#include "Matrice.h"
#include "Modele.h"


struct pixel {
    float x;
    float y;
    float z;
    float colorX;
    float colorY;
    float intensite;
    TGAColor color;
    TGAColor colorN;
    TGAColor colorSpec;
    TGAColor colorGlow;
};

struct matrices{
    Matrice matrice_M;
    Matrice matrice_MIT;
};

const int width = 800;
const int heigth = 800;
const int depth = 255;
const Matrice matrice(4,4);

const Vecteur center(0, 0, 0);
const Vecteur up(0, 1, 0);
const Vecteur light(0,0,3);
const Vecteur eye(1, 0, 3);

using namespace std;

class Dessin {
public:
    Vecteur n;
    Vecteur l;
    float diff;
    float specf;
    Dessin();
    Vecteur barycentrique(pixel p, pixel p1, pixel p2, pixel p3);
    bool isInTriangle(Vecteur vecteur);
    std::vector<pixel> findbox(pixel pt1, pixel pt2, pixel pt3);
    TGAColor interpolateTriangle(Vecteur v, pixel p1, pixel p2, pixel p3, TGAImage &image, pixel newPt);
    float interpolateIntensite(pixel newPt, matrices);
    TGAColor convertirIntensite(pixel pixel);
    void settriangle(vector<pixel> screen, TGAImage &image, float *zbuffer, Modele *modele, matrices matrice);
    void interpolateSpec(pixel newPt);
    Vecteur matriceTovecteur(Matrice m);
};


#endif //MDRPROJET_DESSIN_H
