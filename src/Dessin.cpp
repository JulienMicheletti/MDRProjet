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


float Dessin::interpolateIntensite(pointf newPt, matrices matrice){
    Vecteur normal(newPt.colorN.bgra[2], newPt.colorN.bgra[1], newPt.colorN.bgra[0]);
    Vecteur n;
    Vecteur l;
    Vecteur r;
    Matrice m(4,1);
    normal.x /= 127;
    normal.y /= 127;
    normal.z /= 127;
    normal.x -= 1;
    normal.y -= 1;
    normal.z -= 1;
    m = m.convertir(normal);
    m = matrice.matrice_MIT.multiplier(m);
    n.x = m.getMatrice()[0][0];
    n.y = m.getMatrice()[1][0];
    n.z = m.getMatrice()[2][0];

    Vecteur light(0, 0, 1);

    m = m.convertir(light);
    m = matrice.matrice_M.multiplier(m);
    l.x = m.getMatrice()[0][0];
    l.y = m.getMatrice()[1][0];
    l.z = m.getMatrice()[2][0];
    l = l.normalize();

    return  max(n.produitScal(l), 0.0f);
}

float Dessin::interpolateSpec(pointf newPt) {
    Vecteur normal(newPt.colorN.bgra[2], newPt.colorN.bgra[1], newPt.colorN.bgra[0]);
    Vecteur spec(newPt.colorSpec.bgra[2], newPt.colorSpec.bgra[1], newPt.colorSpec.bgra[0]);

    normal.x /= 127;
    normal.y /= 127;
    normal.z /= 127;
    normal.x -= 1;
    normal.y -= 1;
    normal.z -= 1;

 /*   spec.x /= 127;
    spec.y /= 127;
    spec.z /= 127;
    spec.x -= 1;
    spec.y -= 1;
    spec.z -= 1;*/

    Vecteur light(1, 1, 1);
    light = light.normalize();
    Vecteur mult = normal.normal(light);
    mult.x *= 2.f;
    mult.y *=2.f;
    mult.z *=2.f;
   Vecteur r = normal.normal(mult).moins(light).normalize();
    float specf = pow(std::max(r.z, 0.0f), spec.z);
   //cout << specf << endl;
 /* cout << " ";
    cout << spec.y;
    cout << " ";
    cout << spec.z << endl;*/


}


TGAColor Dessin::convertirIntensite(pointf pixel){
    TGAColor color = pixel.color;
    TGAColor newColor((float)color.bgra[2] * pixel.intensite, (float)color.bgra[1]* pixel.intensite, (float)color.bgra[0]* pixel.intensite, 255);
    return newColor;
}


void Dessin::settriangle(vector<pointf> screen, TGAImage &image, float *zbuffer, TGAImage &tgaImage, TGAImage &imageDiffuse, TGAImage &imageSpec, matrices matrices) {
    vector <pointf> box = findbox(screen[0], screen[1], screen[2]);
    Vecteur v;
    float z;
    pointf newPt;

    for (int i = box[1].x; i < box[0].x; i++){
        for (int j = box[1].y; j < box[0].y; j++) {
            newPt.x = i;
            newPt.y = j;
            v = barycentrique(newPt, screen[0], screen[1], screen[2]);
            if (isInTriangle(v)) {
                newPt.z = screen[0].z * v.x + screen[1].z * v.y + screen[2].z * v.z ;
                if (int(newPt.x + newPt.y * width) > 0 && zbuffer[int(newPt.x + newPt.y * width)] < newPt.z) {
                    zbuffer[int(newPt.x + newPt.y * width)] = newPt.z;
                    newPt.color = interpolateTriangle(v, screen[0], screen[1], screen[2], tgaImage, newPt);
                    newPt.colorN = interpolateTriangle(v, screen[0], screen[1], screen[2], imageDiffuse, newPt);
                  //  newPt.colorSpec = interpolateTriangle(v, screen[0], screen[1], screen[2], imageSpec, newPt);
                    newPt.intensite = interpolateIntensite(newPt, matrices);
                  //  interpolateSpec(newPt);
                    newPt.color = convertirIntensite(newPt);
                    image.set(newPt.x, newPt.y, newPt.color);
              }
            }
        }
    }
}