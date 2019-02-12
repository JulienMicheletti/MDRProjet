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
#include <chrono>
#include <cstring>
#include <zconf.h>

vector<vector<string> > points;
vector<int> lignes;
vector<vector<std::string> > textures;
vector<vector<std::string> > intensite;
using namespace std::chrono;
Vecteur eye(1,1,3);
float *zbuffer;

vector<int> readLine(string filename) {
    ifstream fichier(filename.c_str(), ios::in);
    std::vector<int> tab;
    if (fichier) {
        string line;
        string esp;
        string token;
        while (getline(fichier, line)) {
            if (line[0] == 'f') {
                std::istringstream iss(line);
                while (getline(iss, esp, ' ')) {
                    if (esp[0] != 'f') {
                        std::istringstream iss2(esp);
                        for (int i = 0; i < 2; i++) {
                            getline(iss2, token, '/');
                            tab.push_back(-1 + atoi((token).c_str()));
                        }
                    }
                }
            }
        }
        fichier.close();
    } else {
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    return tab;
}

std::vector<vector<std::string> > readPoint(string filename, int type){
    ifstream fichier(filename.c_str(), ios::in);
    std::vector<std::vector<std::string> > tab;
    std::vector<std::string> tmp;
    if (fichier){
        string line;
        while(getline(fichier, line)){
            std::istringstream iss(line);
            std::string token;
            while(std::getline(iss, token, ' ')){
                tmp.push_back(token);
            }
            if (line[0] == 'v' && line[1] != 't' && type == 0){
                tab.push_back(tmp);
            }
            if (line[0] == 'v' && line[1] == 't' && type == 1){
                tab.push_back(tmp);
            }
            if (line[0] == 'v' && line[1] == 'n' && type == 2){
                tab.push_back(tmp);
            }
            tmp.clear();
        }
        fichier.close();
    }else{
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    return tab;
}


Matrice createCordMatrice(pointf p, Matrice matrice){
    matrice.getMatrice()[0][0] = p.x;
    matrice.getMatrice()[1][0] = p.y;
    matrice.getMatrice()[2][0] = p.z;
    matrice.getMatrice()[3][0] = 1.f;
    return matrice;
}


pointf m2v (Matrice m){
    pointf p;
    p.x = m.getMatrice()[0][0]/m.getMatrice()[3][0];
    p.y = m.getMatrice()[1][0]/m.getMatrice()[3][0];
    p.z = m.getMatrice()[2][0]/m.getMatrice()[3][0];
    return p;
}

vector<TGAImage> load_texture(char **tab, int ind){
    vector<TGAImage> imgs;
    char *filename;
    for (int i = 1; i < ind; i++) {
        TGAImage image;
        if (strcmp(tab[i], "african_head.txt") != 0 && strcmp(tab[i], "eye.txt") != 0 && strcmp(tab[i], "african_eye_outer.txt") != 0) {
            filename = tab[i];
            image.read_tga_file(filename);
            image.flip_vertically();
            imgs.push_back(image);
        }
    }
    return imgs;
}

void load_txt(char * filename){
    ::points = readPoint(filename, 0);
    ::lignes = readLine(filename);
    ::textures = readPoint(filename, 1);
    ::intensite = readPoint(filename, 2);

}

void afficher(TGAImage &image, TGAImage &imagetga, TGAImage &imagenm, TGAImage &imagespec) {
    vector<pointf> screen;
    Dessin dessin;
    pointf p;
    matrices matrices;

    Matrice vp = Matrice::viewPort(800, 800, 255);
    Matrice projection = Matrice::projection(eye, center);
    Matrice modelView = Matrice::lookat(eye, center, up);
    matrices.matrice_M = Matrice::matrice_M(projection, modelView);
    matrices.matrice_MIT = Matrice::matrice_MIT(matrices.matrice_M);

    for (int i = 5; i < lignes.size(); i += 6) {
        for (int j = 5; j >= 0; j -= 2) {
            p.x = strtof(points[lignes[i - j]][1].c_str(), 0);
            p.y = strtof(points[lignes[i - j]][2].c_str(), 0);
            p.z = strtof(points[lignes[i - j]][3].c_str(), 0);
            p = m2v(vp.multiplier(projection).multiplier(modelView).multiplier(createCordMatrice(p, matrice)));
            p.colorX = strtof(textures[lignes[i - j + 1]][2].c_str(), 0);
            p.colorY = strtof(textures[lignes[i - j + 1]][3].c_str(), 0);
            screen.push_back(p);
        }
        dessin.settriangle(screen, image, zbuffer, imagetga, imagenm, imagespec, matrices);
        screen.clear();
    }
}

int main(int ac, char **av) {
    TGAImage image(800, 800, TGAImage::RGB);
    vector<TGAImage> imgs;

    imgs = load_texture(av, ac);


    zbuffer = new float[width * heigth];
    for (int i=width*heigth; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    load_txt(av[1]);
    afficher(image, imgs[0], imgs[1], imgs[2]);
    load_txt(av[2]);
    afficher(image, imgs[3], imgs[4], imgs[5]);

    image.flip_vertically();
    image.write_tga_file("output2.tga");

    return 0;
}

