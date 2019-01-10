#include "pch.h"
#include "tgaimage.h"


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
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
            y += (y1>y0?1:-1); 
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

void afficher(std::vector<vector<std::string> > points, vector<int> lignes){
  int x0, x1, y0, y1 = 0;
  int id = 1;
  TGAImage image(500, 500, TGAImage::RGB);
  for (int i = 0; i < lignes.size(); i++) {
	  if (id == 1 || id == 2) {
		  x0 = strtof(points[lignes[i]][1].c_str(), 0) * 250 + 250;
		  y0 = strtof(points[lignes[i]][2].c_str(), 0) * 250 + 250;
		  x1 = strtof(points[lignes[i+1]][1].c_str(), 0) * 250 + 250;
		  y1 = strtof(points[lignes[i+1]][2].c_str(), 0) * 250 + 250;
		  id++;
	  }
	  else if (id == 3) {
		  x0 = strtof(points[lignes[i]][1].c_str(), 0) * 250 + 250;
		  y0 = strtof(points[lignes[i]][2].c_str(), 0) * 250 + 250;
		  x1 = strtof(points[lignes[i - 2]][1].c_str(), 0) * 250 + 250;
		  y1 = strtof(points[lignes[i - 2]][2].c_str(), 0) * 250 + 250;
		  id = 1;
	  }
	  line(x0, y0, x1, y1, image, white);
  }
 image.flip_vertically();
 image.write_tga_file("output.tga");
}



int main(int ac, char **av){
  string filename = "african_head.txt";
  afficher(readPoint(filename), readLine(filename));
  

 return 0;
}
