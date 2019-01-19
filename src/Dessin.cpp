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

Vecteur barycentrique(pointf p, pointf p1, pointf p2, pointf p3){
    Vecteur w;

    w.x = (p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y);
    w.x /= (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);

    w.y = (p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y);
    w.y /= (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);

    w.z = 1 - w.x - w.y;
    return w;
}

bool Dessin::isInTriangle(Vecteur vecteur)
{
    if (vecteur.x < 0 || vecteur.y < 0 || vecteur.z < 0){
        return false;
    }
    return true;
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

TGAColor interpolateTriangle(Vecteur v, pointf p1, pointf p2, pointf p3){
    float moyenneR = ((float)p1.color.bgra[0] * v.x + (float)p2.color.bgra[0] * v.y + (float)p3.color.bgra[0] * v.z);
    float moyenneG = ((float)p1.color.bgra[1] * v.x + (float)p2.color.bgra[1] * v.y + (float)p3.color.bgra[1] * v.z);
    float moyenneB = ((float)p1.color.bgra[2] * v.x + (float)p2.color.bgra[2] * v.y + (float)p3.color.bgra[2] * v.z);
    TGAColor newColor(moyenneR, moyenneG, moyenneB, 255);
    return newColor;
}


void Dessin::settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image, float *zbuffer) {
    vector <pointf> box = findbox(pt1, pt2, pt3);
    Vecteur v;
    float z;
    pointf newPt;

    for (int i = box[1].x; i < box[0].x; i++){
        for (int j = box[1].y; j < box[0].y; j++) {
            newPt.x = i;
            newPt.y = j;
            v = barycentrique(newPt, pt1, pt2, pt3);
            if (isInTriangle(v)) {
                newPt.color = interpolateTriangle(v, pt1, pt2, pt3);
                z = pt1.z * v.x + pt2.z * v.y + pt3.z * v.z ;
                if (zbuffer[int(newPt.x + newPt.y * width)] < z) {
                    zbuffer[int(newPt.x + newPt.y * width)] = z;
                    image.set(newPt.x, newPt.y, newPt.color);
              }
            }
        }
    }
}