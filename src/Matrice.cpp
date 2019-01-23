#include <cstdio>
#include "Matrice.h"
#include "Dessin.h"

Matrice::Matrice(pointf pointf) {
    struct pointf camera;
    camera.x = 0;
    camera.y = 0;
    camera.z = 3;
    this->pointf1 = pointf;
    matrice[0][0] = pointf.x;
    matrice[1][0] = pointf.y;
    matrice[2][0] = pointf.z;
    matrice[3][0] = 1.f;

    c = camera.z;

    for (int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if (i == j){
                matriceMult[i][j] = 1;
                matriceDim[i][j] = 1;
            }else if (i == 3 && j == 2){
                matriceMult[i][j] = -1/c;
                matriceDim[i][j] = -1/c;
            }else{
                matriceMult[i][j] = 0;
                matriceDim[i][j] = 0;
            }
        }
    }

}



pointf Matrice::calculerMatrice(){
    float dimX = 500/8;
    float dimY = 500/8;
    float dimW = 500*3/4;
    float dimH = 500*3/4;
    matriceDim[0][3] = dimX + dimW / 2.f;
    matriceDim[1][3] = dimY + dimH / 2.f;
    matriceDim[2][3] = 255 / 2.f;

    matriceDim[0][0] = dimW / 2.f;
    matriceDim[1][1] = dimH / 2.f;
    matriceDim[2][2] = 255/2.f;

    int result1[4][4];
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
          result1[i][j] = 0.f;
          for (int k = 0; k < 4; k++){
           result1[i][j] += matriceMult[i][k] * matriceDim[k][j];
          }
        }
    }

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 1; j++){
            result[i][j] = 0.f;
            for (int k = 0; k < 4; k++){
                result[i][j] += result1[i][k] * matrice[k][j];
            }
        }
    }


    pointf newPoint;
    newPoint.x = pointf1.x/(1.f-pointf1.z/c);
    newPoint.y = pointf1.y/(1.f-pointf1.z/c);
    newPoint.z = pointf1.z/(1.f-pointf1.z/c);
   // cout << 1.f - pointf1.z/c << endl;

    return newPoint;


}

float Matrice::getC(){
    return c;
}




