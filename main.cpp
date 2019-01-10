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

std::vector<vector<std::string> > readfile(string filename){
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

void afficher(std::vector<vector<std::string> > tab){
  int x = 0;
  int y = 0;
  TGAImage image(500, 500, TGAImage::RGB);
    for (int i = 0; i < tab.size(); i++){
	if (tab[i][0] == "v"){
	  image.set(strtof(tab[i][1].c_str(), 0)*250+250 ,strtof(tab[i][2].c_str(), 0)*250+250, white);
	}
      }
 image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
 image.write_tga_file("output.tga");
}


int main(int ac, char **av){
  string filename = "african_head.txt";
  afficher(readfile(filename));
 return 0;
}
