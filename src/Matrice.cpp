#include <iostream>
#include "Matrice.h"


Matrice matrice1(3,3);
Matrice matrice2(3,3);
Matrice matrice3(3,3);
Matrice matrice4(3,3);


Matrice matrice5(2,2);
Matrice matrice6(2,2);
Matrice matrice7(2,2);

Matrice::Matrice(){
    this->colonnes = 4;
    this->lignes = 4;
    matrice = new float*[colonnes];
    for(int i = 0;i < lignes; i++) matrice[i] = new float[lignes];
}

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
    cout << lignes << endl;
    Matrice transposee(lignes, colonnes);
    for (int i = 0; i < lignes; i++){
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

Matrice Matrice::matrice_M(Matrice projection, Matrice modelView){
    return projection.multiplier(modelView);
}

Matrice Matrice::matrice_MIT(Matrice matrice_M){
    float det = matrice_M.determinant44();
    return matrice_M.comatrice().transposer().inverser(det).transposer();

}

Matrice Matrice::comatrice(){
    Matrice comatrice(4,4);
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {
            comatrice.getMatrice()[i][j] = pow(-1, i + j + 2) * concat(i, j).determinant33();
        }
    }
    return comatrice;
   // inverse.afficher();
}

Matrice Matrice::inverser(float det){
    Matrice inverse(4,4);
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
           inverse.getMatrice()[i][j] = getMatrice()[i][j] / det;
        }
    }
    return inverse;
    // inverse.afficher();
}

Matrice Matrice::concat(int x, int y){
    Matrice matrice(3,3);
    int k = 0;
    int l = 0;

    for (int i = 0; i < 3; i++){
        k = 0;
        if (i == x)
            l++;
        for (int j = 0; j < 3; j++){
            if (j == y)
                k++;
            matrice.getMatrice()[i][j] = getMatrice()[l][k];
            k++;
        }
        l++;
    }
    return matrice;
}

Matrice Matrice::convertir(Vecteur v){
    Matrice matrice(4,1);
    matrice.getMatrice()[0][0] = v.x;
    matrice.getMatrice()[1][0] = v.y;
    matrice.getMatrice()[2][0] = v.z;
    return matrice;
}

float Matrice::determinant33(){
    for (int i = 1; i < 3; i++){
        for (int j = 1; j < 3; j++){
            matrice5.getMatrice()[i-1][j-1] = getMatrice()[i][j];
        }
        matrice6.getMatrice()[i-1][0] = getMatrice()[i][0];
        matrice6.getMatrice()[i-1][1] = getMatrice()[i][2];
        matrice7.getMatrice()[i-1][0] = getMatrice()[i][0];
        matrice7.getMatrice()[i-1][1] = getMatrice()[i][1];
    }

    return matrice5.calculer22(getMatrice()[0][0]) + matrice6.calculer22(-getMatrice()[0][1]) + matrice7.calculer22(getMatrice()[0][2]);
}

float Matrice::determinant44(){
    float res = 0;
    for (int i = 1; i < 4; i++){
        for (int j = 1; j < 4; j++){
            matrice1.getMatrice()[i-1][j-1] = getMatrice()[i][j];
        }
        matrice2.getMatrice()[i-1][0] = getMatrice()[i][0];
        matrice2.getMatrice()[i-1][1] = getMatrice()[i][2];
        matrice2.getMatrice()[i-1][2] = getMatrice()[i][3];
        matrice3.getMatrice()[i-1][0] = getMatrice()[i][0];
        matrice3.getMatrice()[i-1][1] = getMatrice()[i][1];
        matrice3.getMatrice()[i-1][2] = getMatrice()[i][3];
        matrice4.getMatrice()[i-1][0] = getMatrice()[i][0];
        matrice4.getMatrice()[i-1][1] = getMatrice()[i][1];
        matrice4.getMatrice()[i-1][2] = getMatrice()[i][2];
    }
    matrice1.determinant33();
    res += getMatrice()[0][0] * (matrice5.calculer22(matrice1.getMatrice()[0][0]) + matrice6.calculer22(-matrice1.getMatrice()[0][1]) + matrice7.calculer22(matrice1.getMatrice()[0][2]));
    matrice2.determinant33();
    res += -getMatrice()[0][1] * (matrice5.calculer22(matrice2.getMatrice()[0][0]) + matrice6.calculer22(-matrice2.getMatrice()[0][1]) + matrice7.calculer22(matrice2.getMatrice()[0][2]));
    matrice3.determinant33();
    res += getMatrice()[0][2] * (matrice5.calculer22(matrice3.getMatrice()[0][0]) + matrice6.calculer22(-matrice3.getMatrice()[0][1]) + matrice7.calculer22(matrice3.getMatrice()[0][2]));
    matrice4.determinant33();
    res += -getMatrice()[0][3] * (matrice5.calculer22(matrice4.getMatrice()[0][0]) + matrice6.calculer22(-matrice4.getMatrice()[0][1]) + matrice7.calculer22(matrice4.getMatrice()[0][2]));


    return res;
}

float Matrice::calculer22(float pref){
    return pref * ((getMatrice()[0][0] * getMatrice()[1][1]) - (getMatrice()[1][0] * getMatrice()[0][1]));
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




