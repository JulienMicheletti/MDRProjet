#include <cstdio>
#include "Matrice.h"
#include "Dessin.h"

Matrice::Matrice(pointf pointf) {
    matrice[1][0] = pointf.x;
    matrice[2][0] = pointf.y;
    matrice[3][0] = pointf.z;
    matrice[4][0] = 1;
}

Matrice::multiply(){

}


