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


struct pointf {
    float x;
    float y;
    float z;
    float colorX;
    float colorY;
    float intensite;
    TGAColor color;
};


const int width = 800;
const int heigth = 800;
const int depth = 255;

using namespace std;

class Dessin {
public:
    Dessin();
    Vecteur barycentrique(pointf p, pointf p1, pointf p2, pointf p3);
    bool isInTriangle(Vecteur vecteur);
    std::vector<pointf> findbox(pointf pt1, pointf pt2, pointf pt3);
    TGAColor interpolateTriangle(Vecteur v, pointf p1, pointf p2, pointf p3, TGAImage &image, pointf newPt);
    float interpolateIntensite(Vecteur v, pointf p1, pointf p2, pointf p3, pointf newPt);
    TGAColor convertirIntensite(pointf pixel);
    void settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image, float *zbuffer, TGAImage &image1);
};


#endif //MDRPROJET_DESSIN_H
