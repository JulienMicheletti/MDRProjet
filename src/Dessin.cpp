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

bool Dessin::isInTriangle(pointf p, pointf p0, pointf p1, pointf p2)
{
    float condition1 = (p.x - p1.x) * (p0.y - p1.y) - (p0.x - p1.x) * (p.y - p1.y);
    float condition2 = (p.x - p2.x) * (p1.y - p2.y) - (p1.x - p2.x) * (p.y - p2.y);
    float condition3 = (p.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p.y - p0.y);
    bool negatif = false;
    bool positif = false;

    if (condition1 < 0 || condition2 < 0 || condition3 < 0){
        negatif = true;
    }
    if (condition1 > 0 || condition2 > 0 || condition3 > 0){
        positif = true;
    }

    return !(negatif && positif);
}

void Dessin::settriangle(pointf pt1, pointf pt2, pointf pt3, TGAImage &image,  TGAColor color) {
    line(pt1.x, pt1.y, pt2.x, pt2.y, image, color);
    line(pt2.x, pt2.y, pt3.x, pt3.y, image, color);
    line(pt1.x, pt1.y, pt3.x, pt3.y, image, color);
    pointf newPt;

    for (int i = 0; i < 500; i++){
        for (int j = 0; j < 500; j++){
            newPt.x = i;
            newPt.y = j;
            if (isInTriangle(newPt, pt1, pt2, pt3)){
                image.set(i, j, color);
            }
        }
    }
}
