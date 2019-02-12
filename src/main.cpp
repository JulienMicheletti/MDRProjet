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

vector<vector<string> > points;
vector<int> lignes;
vector<vector<std::string> > textures;
vector<vector<std::string> > intensite;
Vecteur eye(1, 1, 3);
using namespace std::chrono;
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

vector<TGAImage> chargerTextures(string filename){
    vector<TGAImage> imgs;
    points = readPoint(filename, 0);
    lignes = readLine(filename);
    textures = readPoint(filename, 1);
    intensite = readPoint(filename, 2);
    TGAImage image1;
    TGAImage image2;
    TGAImage image3;

    string name = filename + "_diffuse.tga";
    string name2 = filename + "_nm.tga";
    string name3 = filename + "_spec.tga";

    image1.read_tga_file(name.c_str());
    image1.flip_vertically();
    image2.read_tga_file(name2.c_str());
    image2.flip_vertically();
    image3.read_tga_file(name3.c_str());
    image3.flip_vertically();

    imgs.push_back(image1);
    imgs.push_back(image2);
    imgs.push_back(image3);
    return imgs;
}

void cleanTexture(vector<TGAImage> texts){
    for (TGAImage image : texts){
        image.clear();
    }
}

int main(int ac, char **av) {
    TGAImage image(800, 800, TGAImage::RGB);
    vector <TGAImage> imgs;

   zbuffer = new float[width * heigth];
   for (int i=width*heigth; i--; zbuffer[i] = -std::numeric_limits<float>::max());

    imgs = chargerTextures(string(av[1]));
    afficher(image, imgs[0], imgs[1], imgs[2]);
    cleanTexture(imgs);
    imgs.clear();

    imgs = chargerTextures(string(av[2]));
    afficher(image, imgs[0], imgs[1], imgs[2]);


    image.flip_vertically();
    image.write_tga_file("output2.tga");
    delete [] zbuffer;

    return 0;
}