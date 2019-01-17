//
// Created by Julien on 16/01/2019.
//

#include "Dessin.h"
#include "main.h"

Dessin::Dessin(){}

void Dessin::line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
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

Vecteur getTriangleVecteur(pointf p, pointf p0, pointf p1, pointf p2){
    Vecteur vecteur;

    vecteur.x = (p.x - p1.x) * (p0.y - p1.y) - (p0.x - p1.x) * (p.y - p1.y);
    vecteur.y = (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
    vecteur.z = (p.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p.y - p0.y);

    return vecteur;
}

bool Dessin::isInTriangle(Vecteur vecteur)
{
   bool negatif = false;
   bool positif = false;

    if (vecteur.x < 0 || vecteur.y < 0 || vecteur.z < 0){
        negatif = true;
    }
    if (vecteur.x > 0 || vecteur.y > 0 || vecteur.z > 0){
        positif = true;
    }

    return !(negatif && positif);
}

vector<pointf> findbox(pointf pt1, pointf pt2, pointf pt3){
    vector<pointf> box;
    pointf min;
    pointf max;
    max.x = pt1.x > pt2.x ? (pt1.x > pt3.x ? pt1.x : pt3.x) : (pt2.x > pt3.x ? pt2.x : pt3.x);
    max.y = pt1.y > pt2.y ? (pt1.y > pt3.y ? pt1.y : pt3.y) : (pt2.y > pt3.y ? pt2.y : pt3.y);
    min.x = pt1.x < pt2.x ? (pt1.x < pt3.x ? pt1.x : pt3.x) : (pt2.x < pt3.x ? pt2.x : pt3.x);
    min.y = pt1.y < pt2.y ? (pt1.y < pt3.y ? pt1.y : pt3.y) : (pt2.y < pt3.y ? pt2.y : pt3.y);

    box.push_back(max);
    box.push_back(min);
    return box;


}

void Dessin::settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image,  TGAColor color) {
    int *zbuffer = new int[width * heigth];
    vector <pointf> box = findbox(pt1, pt2, pt3);
    Vecteur v;
    float z;
    line(pt1.x, pt1.y, pt2.x, pt2.y, image, color);
    line(pt2.x, pt2.y, pt3.x, pt3.y, image, color);
    line(pt1.x, pt1.y, pt3.x, pt3.y, image, color);
    pointf newPt;
    
    for (int i = box[1].x; i < box[0].x; i++){
        for (int j = box[1].y; j < box[0].y; j++){

            newPt.x = i;
            newPt.y = j;
            v = getTriangleVecteur(newPt, pt1, pt2, pt3);
            if (isInTriangle(v)){
                z = 0;
                z += pt1.z * v.x + pt2.z * v.y + pt3.z * v.z;
             //   if (zbuffer[int(i + j*width)] < z){
                    zbuffer[int(i + j*width)] = z;

                image.set(i, j, color);
                  //image.set(i, j, color);
                //}

            }
        }
    }
}
