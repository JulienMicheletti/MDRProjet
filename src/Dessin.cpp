//
// Created by Julien on 16/01/2019.
//

#include "Dessin.h"

Dessin::Dessin(){}

Vecteur Dessin::barycentrique(pointf p, pointf p1, pointf p2, pointf p3){
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

vector<pointf> Dessin::findbox(pointf pt1, pointf pt2, pointf pt3){
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

TGAColor Dessin::interpolateTriangle(Vecteur v, pointf p1, pointf p2, pointf p3, TGAImage &image, pointf newPt){
    TGAColor color;

    newPt.colorX = (p1.colorX * v.x + p2.colorX * v.y + p3.colorX * v.z)*1024;
    newPt.colorY = (p1.colorY * v.x + p2.colorY * v.y + p3.colorY * v.z)*1024;

    color = image.get(newPt.colorX, newPt.colorY);
    return color;
}

float Dessin::interpolateIntensite(Vecteur v, pointf p1, pointf p2, pointf p3, pointf newPt){
   newPt.intensite = p1.intensite * v.x + p2.intensite * v.y + p3.intensite * v.z;

   return newPt.intensite;
}

TGAColor Dessin::convertirIntensite(pointf pixel){
    TGAColor color = pixel.color;
    TGAColor newColor((float)color.bgra[2] * pixel.intensite, (float)color.bgra[1]* pixel.intensite, (float)color.bgra[0]* pixel.intensite, 255);
    return newColor;
}

void Dessin::settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image, float *zbuffer, TGAImage &tgaImage) {
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
                newPt.z = pt1.z * v.x + pt2.z * v.y + pt3.z * v.z ;
                if (int(newPt.x + newPt.y * width) > 0 && zbuffer[int(newPt.x + newPt.y * width)] < newPt.z) {
                    zbuffer[int(newPt.x + newPt.y * width)] = newPt.z;
                    newPt.color = interpolateTriangle(v, pt1, pt2, pt3, tgaImage, newPt);
                    newPt.intensite = interpolateIntensite(v, pt1, pt2, pt3, newPt);
                    if (newPt.intensite > 0) {
                        newPt.color = convertirIntensite(newPt);
                        image.set(newPt.x, newPt.y, newPt.color);
                    }
              }
            }
        }
    }
}