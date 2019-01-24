#include "main.h"
#include "Matrice.h"

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

std::vector<vector<std::string> > readPoint(string filename, bool type){
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
            if (line[0] == 'v' && line[1] != 't' && type){
                tab.push_back(tmp);
            }
            if (line[0] == 'v' && line[1] == 't' && !type){
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

float intensite(vector<pointf> world) {
    Vecteur vecteur1(world[1].x - world[0].x, world[1].y - world[0].y, world[1].z - world[0].z);
    Vecteur vecteur2(world[2].x - world[0].x, world[2].y - world[0].y, world[2].z - world[0].z);
    Vecteur light(0, 0, 1);
    Vecteur normal;
    normal = vecteur1.normal(vecteur2);
    normal = normal.normalize();

    return normal.produitScal(light);

}

Matrice viewPort(){
    float dimX = 500/8;
    float dimY = 500/8;
    float dimW = 500*3/4;
    float dimH = 500*3/4;
    Matrice m = Matrice::matriceId();
    float** tab = m.getMatrice();
    tab[0][3] = dimX + dimW / 2.f;
    tab[1][3] = dimY + dimH / 2.f;
    tab[2][3] = 255 / 2.f;

    tab[0][0] = dimW / 2.f;
    tab[1][1] = dimH / 2.f;
    tab[2][2] = 255/2.f;

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

pointf division(Matrice m){
    pointf p;
    p.x = m.getMatrice()[0][0]/m.getMatrice()[3][0];
    p.y = m.getMatrice()[1][0]/m.getMatrice()[3][0];
    p.z = m.getMatrice()[2][0]/m.getMatrice()[3][0];
    return p;
}


void afficher(vector<vector<string> > points, vector<int> lignes, vector<vector<std::string> > textures, TGAImage &image, TGAImage &imagetga) {
    vector<pointf> screen;
    vector<pointf> world;
    vector<TGAColor> color;
    Dessin dessin;
    pointf p;
    pointf pf;
    float inte;
    float *zbuffer = new float[width*heigth];
    pointf camera;
    camera.x = 0;
    camera.y = 0;
    camera.z = 1;
    Matrice m = Matrice::matriceId();
    Matrice res1(4,4);
    m.getMatrice()[3][2] = -1.f/camera.z;
    for (int i = 5; i < lignes.size(); i+=6) {
        for (int j = 5; j >= 0; j-=2) {
            p.x = strtof(points[lignes[i - j]][1].c_str(), 0);
            p.y = strtof(points[lignes[i - j]][2].c_str(), 0);
            p.z = strtof(points[lignes[i - j]][3].c_str(), 0);
            res1 = viewPort().multiplier(m);
            p = division(res1.multiplier(createCordMatrice(p)));
             pf.x = strtof(points[lignes[i-j]][1].c_str(), 0);
            pf.y = strtof(points[lignes[i-j]][2].c_str(), 0);
            pf.z = strtof(points[lignes[i-j]][3].c_str(), 0);
            p.colorX =  strtof(textures[lignes[i-j+1]][2].c_str(), 0);
            p.colorY =  strtof(textures[lignes[i-j+1]][3].c_str(), 0);
            screen.push_back(p);
            world.push_back(pf);
        }
        inte = intensite(world);
        if (inte > 0) {
           dessin.settriangle(screen[0], screen[1], screen[2], image, zbuffer, inte, imagetga);
        }
        screen.clear();
        world.clear();
        }
    }

    int main(int ac, char **av) {
        TGAImage image(500, 500, TGAImage::RGB);
        TGAImage image2;
        const char *filenameTGA = "C:\\Users\\Julien\\CLionProjects\\MProjet\\african_head_diffuse.tga";
        string filename = "C:\\Users\\Julien\\CLionProjects\\MProjet\\african_head.txt";
        image2.read_tga_file(filenameTGA);
        image2.flip_vertically();
       afficher(readPoint(filename, true), readLine(filename), readPoint(filename, false), image, image2);
        image.flip_vertically();
        image.write_tga_file("output.tga");

        return 0;
    }