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

    public:
        Matrice(pointf pointf);
    };



#endif //MDRPROJET_MATRICE_H
