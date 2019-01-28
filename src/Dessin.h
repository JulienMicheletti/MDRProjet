//
// Created by Julien on 16/01/2019.
//

#ifndef MDRPROJET_DESSIN_H
#define MDRPROJET_DESSIN_H


#include "tgaimage.h"
#include "main.h"


struct pointf {
    float x;
    float y;
    float z;
    float colorX;
    float colorY;
    TGAColor color;
};

class Dessin {
public:
    Dessin();

    void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
    void settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image, float *zbuffer, float inte, TGAImage &image1);

    bool isInTriangle(Vecteur vecteur);
};


#endif //MDRPROJET_DESSIN_H
