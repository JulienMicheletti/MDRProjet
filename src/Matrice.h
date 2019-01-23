//
// Created by Julien on 22/01/2019.
//

#ifndef MDRPROJET_MATRICE_H
#define MDRPROJET_MATRICE_H


#include <vector>
#include <sstream>
#include "Dessin.h"

class Matrice {
protected:
    int size;
    float matrice[4][1];
    float matriceMult[4][4];
    float matriceDim[4][4];
    float result[4][1];
    pointf pointf1;
    float c;

    public:
    Matrice(pointf);
    pointf calculerMatrice();

    float getC();
    };



#endif //MDRPROJET_MATRICE_H
