#include <cstdio>
#include "Matrice.h"
#include "Dessin.h"

Matrice::Matrice(int lignes, int colonnes) {
    this->colonnes = colonnes;
    this->lignes = lignes;
    matrice = new float*[colonnes];
    for(int i = 0;i < lignes; i++) matrice[i] = new float[lignes];

}

Matrice Matrice::matriceId() {
    Matrice matrice1(4,4);
    float** m = matrice1.getMatrice();
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                m[i][j] = 1;
            } else {
                m[i][j] = 0;
            }
        }
    }
    return matrice1;
}

float** Matrice::getMatrice(){
    return matrice;
}

int Matrice::getLignes() {
    return lignes;
}

int Matrice::getColonnes() {
    return colonnes;
}

Matrice Matrice::multiplier(Matrice m2){
    Matrice res(lignes, m2.getColonnes());
    float** tab = res.getMatrice();
    for (int i = 0; i < m2.getLignes(); i++){
        for (int j = 0; j < m2.getColonnes(); j++){
            tab[i][j] = 0.f;
            for (int k = 0; k < colonnes; k++){
                tab[i][j] += getMatrice()[i][k] * m2.getMatrice()[k][j];
            }
        }
    }
    return res;
}

void Matrice::afficher() {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            cout << matrice[i][j];
            cout << " ";
        }
        cout << " " << endl;
    }
}




