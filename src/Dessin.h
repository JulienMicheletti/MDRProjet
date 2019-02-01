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


struct pointf {
    float x;
    float y;
    float z;
    float colorX;
    float colorY;
    float intensite;
    TGAColor color;
    TGAColor colorN;
};


const int width = 800;
const int heigth = 800;
const int depth = 255;
const Matrice matrice(4,4);

const Vecteur eye(1, 1, 3);
const Vecteur center(0, 0, 0);
const Vecteur up(0, 1, 0);


using namespace std;

class Dessin {
public:
    Dessin();
    Vecteur barycentrique(pointf p, pointf p1, pointf p2, pointf p3);
    bool isInTriangle(Vecteur vecteur);
    std::vector<pointf> findbox(pointf pt1, pointf pt2, pointf pt3);
    TGAColor interpolateTriangle(Vecteur v, pointf p1, pointf p2, pointf p3, TGAImage &image, pointf newPt);
    float interpolateIntensite(Vecteur v, pointf newPt);
    TGAColor convertirIntensite(pointf pixel);
    void settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image, float *zbuffer, TGAImage &image1, TGAImage &imageDiffuse);
};


#endif //MDRPROJET_DESSIN_H
