//
// Created by Julien on 16/01/2019.
//

#include <cmath>
#include "Vecteur.h"

Vecteur::Vecteur(){}

Vecteur::Vecteur(float x, float y, float z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float Vecteur::get(int i){
        if (i == 0){
            return x;
        }else if (i == 1){
            return y;
        }else{
            return z;
        }
}
    Vecteur Vecteur::plus(Vecteur v){
        Vecteur newVect(v.x + x, v.y + y, v.z + z);
        return newVect;
    }

    Vecteur Vecteur::moins(Vecteur v){
        Vecteur newVect(x - v.x, y - v.y, z - v.z);
        return newVect;
}

    Vecteur Vecteur::div (float num)
    {
        return Vecteur(x / num, y / num, z / num);
    }

    Vecteur Vecteur::normal(Vecteur v){
        Vecteur n;

        n.x = ((y * v.z) - (z * v.y));
        n.y = ((z * v.x) - (x * v.z));
        n.z = ((x * v.y) - (y * v.x));

        return n;
    }

    float Vecteur::produitScal(Vecteur v){

        return x * v.x + y * v.y + z * v.z;

    }

    float Vecteur::norme()
    {
        return sqrtf( Vecteur(x,y,z).produitScal(Vecteur(x,y,z)));
    }

    Vecteur Vecteur::normalize() {
        return (Vecteur(x, y, z).div(norme()));
    }
