#include "main.h"
#include "tgaimage.h"
#include "Vecteur.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const int width = 500;
const int heigth = 500;
using namespace std;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1-x0;
    int dy = y1-y0;
    int derror2 = std::abs(dy)*2;
    int error2 = 0;
    int y = y0;
    for (int x=x0; x<=x1; x++) {
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx*2;
        }
    }
}

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
                        getline(iss2, token, '/');
                        tab.push_back(-1 + atoi((token).c_str()));
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

std::vector<vector<std::string> > readPoint(string filename){
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
            if (line[0] == 'v'){
                tab.push_back(tmp);
                tmp.clear();
            }
        }
        fichier.close();
    }else{
        cerr << "Impossible d'ouvrir le fichier !" << endl;
    }
    return tab;
}

bool isInTriangle(pointf p, pointf p0, pointf p1, pointf p2)
{
    float condition1 = (p.x - p1.x) * (p0.y - p1.y) - (p0.x - p1.x) * (p.y - p1.y);
    float condition2 = (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
    float condition3 = (p.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p.y - p0.y);
    bool negatif = false;
    bool positif = false;

    if (condition1 < 0 || condition2 < 0 || condition3 < 0){
        negatif = true;
    }
    if (condition1 > 0 || condition2 > 0 || condition3 > 0){
        positif = true;
    }

    return !(negatif && positif);
}

void settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image,  TGAColor color) {
    line(pt1.x, pt1.y, pt2.x, pt2.y, image, color);
    line(pt2.x, pt2.y, pt3.x, pt3.y, image, color);
    line(pt1.x, pt1.y, pt3.x, pt3.y, image, color);
    pointf newPt;

    for (int i = 0; i < width; i++){
        for (int j = 0; j < heigth; j++){
            newPt.x = i;
            newPt.y = j;
            if (isInTriangle(newPt, pt1, pt2, pt3)){
                image.set(i, j, color);
            }
        }
    }
}

float intensite(vector<pointf> world) {
    Vecteur vecteur1(world[1].x - world[0].x, world[1].y - world[0].y, world[1].z - world[0].z);
    Vecteur vecteur2(world[2].x - world[0].x, world[2].y - world[0].y, world[2].z - world[0].z);
    Vecteur light(0, 0, 50);
    Vecteur normal;
    normal = vecteur1.normal(vecteur2);
    normal.normalize();

    return normal.produitScal(light);

}

void afficher(std::vector<vector<std::string> > points, vector<int> lignes, TGAImage &image) {
    vector<pointf> screen;
    vector<pointf> world;
    pointf p;
    pointf pf;
    float inte;
    for (int i = 2; i < lignes.size(); i+=3) {
        for (int j = 2; j >= 0; j--) {
            p.x = strtof(points[lignes[i-j]][1].c_str(), 0) * 250 + 250;
            p.y = strtof(points[lignes[i-j]][2].c_str(), 0) * 250 + 250;
            pf.x = strtof(points[lignes[i-j]][1].c_str(), 0);
            pf.y = strtof(points[lignes[i-j]][2].c_str(), 0);
            pf.z = strtof(points[lignes[i-j]][3].c_str(), 0);
            screen.push_back(p);
            world.push_back(pf);
        }
        inte = intensite(world);
        if (inte > 0) {
            settriangle(screen[0], screen[1], screen[2], image, TGAColor(inte * 255, inte * 255, inte * 255, 255));
        }
        screen.clear();
        world.clear();
    }
    }

    int main(int ac, char **av) {
        TGAImage image(500, 500, TGAImage::RGB);
        string filename = "C:\\Users\\Julien\\CLionProjects\\MDRProjet\\african_head.txt";
        afficher(readPoint(filename), readLine(filename), image);
        image.flip_vertically();
        image.write_tga_file("output.tga");

        return 0;
    }