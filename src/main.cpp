#include "main.h"
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
    Vecteur light(1, -1, 1);
    Vecteur normal = intensite.normalize();
    light = light.normalize();
    return normal.produitScal(light);

}

Matrice viewPort(){
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

Matrice createCordMatrice(pointf p){
    Matrice matrice(4, 1);

    matrice.getMatrice()[0][0] = p.x;
    matrice.getMatrice()[1][0] = p.y;
    matrice.getMatrice()[2][0] = p.z;
    matrice.getMatrice()[3][0] = 1.f;
    return matrice;
}

Matrice lookat(Vecteur eye, Vecteur centre, Vecteur up){
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

pointf m2v (Matrice m){
    pointf p;
    p.x = m.getMatrice()[0][0]/m.getMatrice()[3][0];
    p.y = m.getMatrice()[1][0]/m.getMatrice()[3][0];
    p.z = m.getMatrice()[2][0]/m.getMatrice()[3][0];
    return p;
}

void afficher(vector<vector<string> > points, vector<int> lignes, vector<vector<std::string> > textures,vector<vector<std::string> > intensite,  TGAImage &image, TGAImage &imagetga) {
    Vecteur eye(1, 1, 3);
    Vecteur center(0, 0, 0);
    vector<pointf> screen;
    vector<pointf> world;
    Dessin dessin;
    pointf p;
    float *zbuffer = new float[width * heigth];
    Matrice projection = Matrice::matriceId();
    Matrice vp = viewPort();

    projection.getMatrice()[3][2] = -1.f / ((eye.moins(center)).norme());
    Matrice modelView = lookat(eye, center, Vecteur(0, 1, 0));

    for (int i = 5; i < lignes.size(); i += 6) {
        for (int j = 5; j >= 0; j -= 2) {
            p.x = strtof(points[lignes[i - j]][1].c_str(), 0);
            p.y = strtof(points[lignes[i - j]][2].c_str(), 0);
            p.z = strtof(points[lignes[i - j]][3].c_str(), 0);
            p = m2v(vp.multiplier(projection).multiplier(modelView).multiplier(createCordMatrice(p)));
            p.colorX = strtof(textures[lignes[i - j + 1]][2].c_str(), 0);
            p.colorY = strtof(textures[lignes[i - j + 1]][3].c_str(), 0);
            Vecteur inte(strtof(intensite[lignes[i - j]][2].c_str(), 0), strtof(intensite[lignes[i - j]][3].c_str(), 0), strtof(intensite[lignes[i - j]][4].c_str(), 0));
            p.intensite = getIntensite(inte);
            screen.push_back(p);
        }
        dessin.settriangle(screen[0], screen[1], screen[2], image, zbuffer, imagetga);
        screen.clear();
        world.clear();
    }
}

    int main(int ac, char **av) {
        TGAImage image(800, 800, TGAImage::RGB);
        TGAImage image2;
        const char *filenameTGA = "C:\\Users\\Julien\\CLionProjects\\MProjet\\african_head_diffuse.tga";
        string filename = "C:\\Users\\Julien\\CLionProjects\\MProjet\\african_head.txt";
        image2.read_tga_file(filenameTGA);
        image2.flip_vertically();
       afficher(readPoint(filename, 0), readLine(filename), readPoint(filename, 1),  readPoint(filename, 2), image, image2);
        image.flip_vertically();
        image.write_tga_file("output.tga");


        return 0;
    }