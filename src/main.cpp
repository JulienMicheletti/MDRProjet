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

float intensite(vector<pointf> world) {
    Vecteur vecteur1(world[1].x - world[0].x, world[1].y - world[0].y, world[1].z - world[0].z);
    Vecteur vecteur2(world[2].x - world[0].x, world[2].y - world[0].y, world[2].z - world[0].z);
    Vecteur light(0, 0, 1);
    Vecteur normal;
    normal = vecteur1.normal(vecteur2);
    float norme = sqrtf( Vecteur(normal.x,normal.y,normal.z).produitScal(Vecteur(normal.x,normal.y, normal.z)));
    Vecteur normalize = normal.div(norme);
    return normalize.produitScal(light);

}

void afficher(std::vector<vector<std::string> > points, vector<int> lignes, TGAImage &image) {
    vector<pointf> screen;
    vector<pointf> world;
    Dessin dessin;
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
            dessin.settriangle(screen[0], screen[1], screen[2], image, TGAColor(inte * 255, inte * 255, inte * 255, 255));
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