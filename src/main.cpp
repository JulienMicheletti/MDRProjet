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


using namespace std::chrono;
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

float getIntensite(Vecteur intensite) {
    Vecteur light(1, 1, 1);
    Vecteur normal = intensite.normalize();
    light = light.normalize();
    return normal.produitScal(light);

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

void afficher(vector<vector<string> > points, vector<int> lignes, vector<vector<std::string> > textures,vector<vector<std::string> > intensite,  TGAImage &image, TGAImage &imagetga, TGAImage &imagenm) {
    vector<pointf> screen;
    Dessin dessin;
    pointf p;

    Matrice vp = Matrice::viewPort(800, 800, 255);
    Matrice projection = Matrice::projection(eye, center);
    Matrice modelView = Matrice::lookat(eye, center, up);
    float *zbuffer = new float[width * heigth];

    for (int i = 5; i < lignes.size(); i += 6) {
        for (int j = 5; j >= 0; j -= 2) {
            p.x = strtof(points[lignes[i - j]][1].c_str(), 0);
            p.y = strtof(points[lignes[i - j]][2].c_str(), 0);
            p.z = strtof(points[lignes[i - j]][3].c_str(), 0);
            p = m2v(vp.multiplier(projection).multiplier(modelView).multiplier(createCordMatrice(p, matrice)));
            p.colorX = strtof(textures[lignes[i - j + 1]][2].c_str(), 0);
            p.colorY = strtof(textures[lignes[i - j + 1]][3].c_str(), 0);
            //Vecteur inte(strtof(intensite[lignes[i - j]][2].c_str(), 0), strtof(intensite[lignes[i - j]][3].c_str(), 0), strtof(intensite[lignes[i - j]][4].c_str(), 0));
            //p.intensite = getIntensite(inte);
            screen.push_back(p);
        }
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        dessin.settriangle(screen[0], screen[1], screen[2], image, zbuffer, imagetga, imagenm);
        screen.clear();
    }
}

    int main(int ac, char **av) {
    high_resolution_clock::time_point t1 = high_resolution_clock::now();
        TGAImage image(800, 800, TGAImage::RGB);
        TGAImage imageDiffuse;
        TGAImage imageNm;

        const char *filenameTGA = "C:\\Users\\Julien\\CLionProjects\\MoteurRenduProjet\\diablo3_pose_diffuse.tga";
        string filename = "C:\\Users\\Julien\\CLionProjects\\MoteurRenduProjet\\diablo.txt";
        const char *nmTga = "C:\\Users\\Julien\\CLionProjects\\MoteurRenduProjet\\diablo3_pose_nm.tga";
        imageDiffuse.read_tga_file(filenameTGA);
        imageDiffuse.flip_vertically();
        imageNm.read_tga_file(nmTga);
        imageNm.flip_vertically();
        afficher(readPoint(filename, 0), readLine(filename), readPoint(filename, 1),  readPoint(filename, 2), image, imageDiffuse, imageNm);
        image.flip_vertically();
        image.write_tga_file("output.tga");
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        auto duration = duration_cast<microseconds> (t2-t1).count();
        cout << duration;


        return 0;
    }