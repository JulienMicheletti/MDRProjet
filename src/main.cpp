#include "main.h"
#include "tgaimage.h"
#include "Vecteur.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
using namespace std;

struct point
{
    int x;
    int y;
    int z;
};

struct pointf
{
    float x;
    float y;
    float z;
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

bool intpoint_inside_trigon(point s, point a, point b, point c)
{
    int as_x = s.x-a.x;
    int as_y = s.y-a.y;

    bool s_ab = (b.x-a.x)*as_y-(b.y-a.y)*as_x > 0;

    if((c.x-a.x)*as_y-(c.y-a.y)*as_x > 0 == s_ab) return false;

    if((c.x-b.x)*(s.y-b.y)-(c.y-b.y)*(s.x-b.x) > 0 != s_ab) return false;

    return true;
}

void settriangle(point pt1, point pt2, point pt3, TGAImage &image,  TGAColor color) {
    line(pt1.x, pt1.y, pt2.x, pt2.y, image, color);
    line(pt2.x, pt2.y, pt3.x, pt3.y, image, color);
    line(pt1.x, pt1.y, pt3.x, pt3.y, image, color);
    point newPt;

    for (int i = 0; i < 500; i++){
        for (int j = 0; j < 500; j++){
            newPt.x = i;
            newPt.y = j;
            if (intpoint_inside_trigon(newPt, pt1, pt2, pt3)){
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
    vector<point> screen;
    vector<pointf> world;
    float inte = 1;
    point p0, p1, p2;
    pointf p3, p4, p5;
    int id = 1;
    for (int i = 0; i < lignes.size(); i++) {
        if (id == 1 || id == 2) {
            id++;
        } else if (id == 3) {
            p0.x = strtof(points[lignes[i]][1].c_str(), 0) * 250 + 250;
            p0.y = strtof(points[lignes[i]][2].c_str(), 0) * 250 + 250;
            p1.x = strtof(points[lignes[i - 2]][1].c_str(), 0) * 250 + 250;
            p1.y = strtof(points[lignes[i - 2]][2].c_str(), 0) * 250 + 250;
            p2.x = strtof(points[lignes[i - 1]][1].c_str(), 0) * 250 + 250;
            p2.y = strtof(points[lignes[i - 1]][2].c_str(), 0) * 250 + 250;
            p3.x = strtof(points[lignes[i]][1].c_str(), 0);
            p3.y = strtof(points[lignes[i]][2].c_str(), 0);
            p3.z = strtof(points[lignes[i]][3].c_str(), 0);
            p4.x = strtof(points[lignes[i - 2]][1].c_str(), 0);
            p4.y = strtof(points[lignes[i - 2]][2].c_str(), 0);
            p4.z = strtof(points[lignes[i - 2]][3].c_str(), 0);
            p5.x = strtof(points[lignes[i - 1]][1].c_str(), 0);
            p5.y = strtof(points[lignes[i - 1]][2].c_str(), 0);
            p5.z = strtof(points[lignes[i - 1]][3].c_str(), 0);
            id = 1;
            screen.push_back(p0);
            screen.push_back(p1);
            screen.push_back(p2);
            world.push_back(p3);
            world.push_back(p4);
            world.push_back(p5);

            inte = intensite(world);
            if (inte > 0) {
                settriangle(p0, p1, p2, image, TGAColor(inte * 255, inte * 255, inte * 255, 255));
            }
            screen.clear();
            world.clear();
        }
    }
    }

    int main(int ac, char **av) {
        TGAImage image(500, 500, TGAImage::RGB);
        string filename = "C:\\Users\\Julien\\CLionProjects\\MDRProjet\\african_head.txt";
        afficher(readPoint(filename), readLine(filename), image);
        //settriangle(100, 100, 300, 200, 50, 100, image, TGAColor(rand()%255, rand()%255, rand()%255, 255));
        image.flip_vertically();
        image.write_tga_file("output.tga");

        return 0;
    }