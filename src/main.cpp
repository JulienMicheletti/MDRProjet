#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>

#include "tgaimage.h"
#include "Vecteur.h"
#include "Dessin.h"
#include "Matrice.h"
#include "Modele.h"
#include <chrono>
#include <cstring>
#include <curses.h>

float *zbuffer;
Modele *modele;

Matrice createCordMatrice(pixel p, Matrice matrice){
    matrice.getMatrice()[0][0] = p.x;
    matrice.getMatrice()[1][0] = p.y;
    matrice.getMatrice()[2][0] = p.z;
    matrice.getMatrice()[3][0] = 1.f;
    return matrice;
}

pixel m2p (Matrice m){
    pixel p;
    p.x = m.getMatrice()[0][0]/m.getMatrice()[3][0];
    p.y = m.getMatrice()[1][0]/m.getMatrice()[3][0];
    p.z = m.getMatrice()[2][0]/m.getMatrice()[3][0];
    return p;
}

void afficher(TGAImage &image) {
    vector<pixel> screen;
    Dessin dessin;
    pixel p;
    matrices matrices;

    Matrice vp = Matrice::viewPort(800, 800, 255);
    Matrice projection = Matrice::projection(eye, center);
    Matrice modelView = Matrice::lookat(eye, center, up);
    matrices.matrice_M = Matrice::matrice_M(projection, modelView);
    matrices.matrice_MIT = Matrice::matrice_MIT(matrices.matrice_M);


    for (int i = 5; i < modele->lignes.size(); i += 6) {
        for (int j = 5; j >= 0; j -= 2) {
            p.x = strtof(modele->points[modele->lignes[i - j]][1].c_str(), 0);
            p.y = strtof(modele->points[modele->lignes[i - j]][2].c_str(), 0);
            p.z = strtof(modele->points[modele->lignes[i - j]][3].c_str(), 0);
            p = m2p(vp.multiplier(projection).multiplier(modelView).multiplier(createCordMatrice(p, matrice)));
            p.colorX = strtof(modele->textures[modele->lignes[i - j + 1]][2].c_str(), 0);
            p.colorY = strtof(modele->textures[modele->lignes[i - j + 1]][3].c_str(), 0);
            screen.push_back(p);
        }
        dessin.settriangle(screen, image, zbuffer, modele, matrices);
        screen.clear();
    }
}

int main(int ac, char **av) {
    TGAImage image(800, 800, TGAImage::RGB);
    TGAColor grey(192,192,192);
    for (int i = 0; i < width; i++){
        for (int j = 0; j < heigth; j++){
            image.set(i, j, grey);
        }
    }

   zbuffer = new float[width * heigth];
   for (int i=width*heigth; i--; zbuffer[i] = -std::numeric_limits<float>::max());
       for (int i = 1; i < ac; i++) {
           string name = string(av[i]);
           modele = new Modele(name, false);
           afficher(image);
       }

       image.flip_vertically();
       image.write_tga_file("african.tga");
       delete[] zbuffer;

    return 0;
}