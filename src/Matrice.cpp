#include <iostream>
#include "Matrice.h"

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
    for (int i = 0; i < lignes; i++){
        for (int j = 0; j < m2.getColonnes(); j++){
            res.getMatrice()[i][j] = 0.f;
            for (int k = 0; k < colonnes; k++){
                res.getMatrice()[i][j] += getMatrice()[i][k] * m2.getMatrice()[k][j];
            }
        }
    }
    return res;
}

Matrice Matrice::transposer(){
    Matrice transposee(lignes, colonnes);
    for (int i = 0; i <lignes; i++){
        for (int j = 0; j < colonnes; j++){
            transposee.getMatrice()[j][i] = getMatrice()[i][j];
        }
    }
    return transposee;
}


Matrice Matrice::viewPort(float width, float heigth, float depth){
    float dimX = width/8;
    float dimY = heigth/8;
    float dimW = width*3/4;
    float dimH = heigth*3/4;
    Matrice m = Matrice::matriceId();
    m.getMatrice()[0][3] = dimX + dimW / 2.f;
    m.getMatrice()[1][3] = dimY + dimH / 2.f;
    m.getMatrice()[2][3] = depth / 2.f;

    m.getMatrice()[0][0] = dimW / 2.f;
    m.getMatrice()[1][1] = dimH / 2.f;
    m.getMatrice()[2][2] = depth / 2.f;

    return m;

}
Matrice Matrice::lookat(Vecteur eye, Vecteur centre, Vecteur up){
    Vecteur z = (eye.moins(centre)).normalize();
    Vecteur x = (up.normal(z)).normalize();
    Vecteur y = (z.normal(x)).normalize();
    Matrice minV(4,4);
    minV = minV.matriceId();
    for (int i = 0; i<3; i++){
        minV.getMatrice()[0][i] = x.get(i);
        minV.getMatrice()[1][i] = y.get(i);
        minV.getMatrice()[2][i] = z.get(i);
        minV.getMatrice()[i][3] = -centre.get(i);
    }
    return minV;

}

Matrice Matrice::projection(Vecteur eye, Vecteur center){
    Matrice matrice = matriceId();
    matrice.getMatrice()[3][2] = -1.f / ((eye.moins(center)).norme());
    return matrice;
}

void Matrice::afficher() {
    for (int i = 0; i < lignes; i++) {
        for (int j = 0; j < colonnes; j++) {
            std::cout << matrice[i][j];
            std::cout << " ";
        }
        cout << " " << endl;
    }
}




