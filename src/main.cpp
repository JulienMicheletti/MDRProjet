#include "main.h"
#include "tgaimage.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
using namespace std;

struct point
{
    int x;
    int y;
};

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
float Sign(point p1, point p2, point p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool IsPointInTri(point pt, point v1, point v2, point v3)
{
    bool b1, b2, b3;

    b1 = Sign(pt, v1, v2) < 0.0f;
    b2 = Sign(pt, v2, v3) < 0.0f;
    b3 = Sign(pt, v3, v1) < 0.0f;

    return ((b1 == b2) && (b2 == b3));
}

void settriangle(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image,  TGAColor color) {
    line(x0, y0, x1, y1, image, color);
    line(x1, y1, x2, y2, image, color);
    line(x0, y0, x2, y2, image, color);
    point newPt;
    point pt1, pt2, pt3;
    pt1.x = x0;
    pt1.y = y0;
    pt2.x = x1;
    pt2.y = y1;
    pt3.x = x2;
    pt3.y = y2;

 for (int i = 0; i < image.get_width(); i++){
        for (int j = 0; j < image.get_height(); j++){
            newPt.x = i;
            newPt.y = j;
            if (IsPointInTri(newPt, pt1, pt2, pt3)){
                image.set(i, j, color);
            }
        }
    }
}

void afficher(std::vector<vector<std::string> > points, vector<int> lignes, TGAImage &image){
    int x0, x1, y0, y1, x2, y2 = 0;
    int id = 1;
    for (int i = 0; i < lignes.size(); i++) {
        if (id == 1 || id == 2) {
            id++;
        }
        else if (id == 3) {
            x0 = strtof(points[lignes[i]][1].c_str(), 0) * 250 + 250;
            y0 = strtof(points[lignes[i]][2].c_str(), 0) * 250 + 250;
            x1 = strtof(points[lignes[i - 2]][1].c_str(), 0) * 250 + 250;
            y1 = strtof(points[lignes[i - 2]][2].c_str(), 0) * 250 + 250;
            x2 = strtof(points[lignes[i - 1]][1].c_str(), 0) * 250 + 250;
            y2 = strtof(points[lignes[i - 1]][2].c_str(), 0) * 250 + 250;
            id = 1;
            settriangle(x0, y0, x1, y1, x2, y2, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
        }

    }
}


int main(int ac, char **av){
    TGAImage image(500, 500, TGAImage::RGB);
    string filename = "C:\\Users\\Julien\\CLionProjects\\MDRProjet\\african_head.txt";
    afficher(readPoint(filename), readLine(filename), image);
    //settriangle(100, 100, 300, 200, 50, 100, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}