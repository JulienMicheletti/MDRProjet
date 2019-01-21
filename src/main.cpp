#include "main.h"

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

void afficher(vector<vector<string> > points, vector<int> lignes, vector<vector<std::string> > textures, TGAImage &image, TGAImage &imagetga) {
    vector<pointf> screen;
    vector<pointf> world;
    vector<TGAColor> color;
    Dessin dessin;
    pointf p;
    pointf pf;
    float inte;
    float *zbuffer = new float[width*heigth];
    for (int i = 5; i < lignes.size(); i+=6) {
        for (int j = 5; j >= 0; j-=2) {
            p.x = strtof(points[lignes[i-j]][1].c_str(), 0) * 250 + 250;
            p.y = strtof(points[lignes[i-j]][2].c_str(), 0) * 250 + 250;
            p.z = strtof(points[lignes[i-j]][3].c_str(), 0) * 250 + 250;
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
        image.write_tga_file("output3.tga");

        return 0;
    }