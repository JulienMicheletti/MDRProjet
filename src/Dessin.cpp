//
// Created by Julien on 16/01/2019.
//

#include "Dessin.h"


Dessin::Dessin(){}

Vecteur Dessin::barycentrique(pixel p, pixel p1, pixel p2, pixel p3){
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

vector<pixel> Dessin::findbox(pixel pt1, pixel pt2, pixel pt3){
    vector<pixel> box;
    pixel min;
    pixel max;
    max.x = pt1.x > pt2.x ? (pt1.x > pt3.x ? pt1.x : pt3.x) : (pt2.x > pt3.x ? pt2.x : pt3.x);
    max.y = pt1.y > pt2.y ? (pt1.y > pt3.y ? pt1.y : pt3.y) : (pt2.y > pt3.y ? pt2.y : pt3.y);
    min.x = pt1.x < pt2.x ? (pt1.x < pt3.x ? pt1.x : pt3.x) : (pt2.x < pt3.x ? pt2.x : pt3.x);
    min.y = pt1.y < pt2.y ? (pt1.y < pt3.y ? pt1.y : pt3.y) : (pt2.y < pt3.y ? pt2.y : pt3.y);

    box.push_back(max);
    box.push_back(min);
    return box;


}

TGAColor Dessin::interpolateTriangle(Vecteur v, pixel p1, pixel p2, pixel p3, TGAImage &image, pixel newPt){
    TGAColor color;

    newPt.colorX = (p1.colorX * v.x + p2.colorX * v.y + p3.colorX * v.z)*image.get_width();
    newPt.colorY = (p1.colorY * v.x + p2.colorY * v.y + p3.colorY * v.z)*image.get_width();//256;//1024;

    color = image.get(newPt.colorX, newPt.colorY);
    return color;
}

Vecteur Dessin::matriceTovecteur(Matrice m){
    Vecteur v;
    v.x = m.getMatrice()[0][0];
    v.y = m.getMatrice()[1][0];
    v.z = m.getMatrice()[2][0];
    return v;
}


float Dessin::interpolateIntensite(pixel newPt, matrices matrice){
    Vecteur normal(newPt.colorN.bgra[2], newPt.colorN.bgra[1], newPt.colorN.bgra[0]);
    Matrice m(4,4);
    normal = Vecteur::convertirRGB(normal);
    m.convertir(normal);
    m = matrice.matrice_MIT.multiplier(m);
    n = matriceTovecteur(m);


    m.convertir(light);
    m = matrice.matrice_M.multiplier(m);
    l = matriceTovecteur(m);
    l = l.normalize();

    return  max(n.produitScal(l), 0.0f);
}

void Dessin::interpolateSpec(pixel newPt) {
    Vecteur spec(newPt.colorSpec.bgra[2], newPt.colorSpec.bgra[1], newPt.colorSpec.bgra[0]);
    float scal = n.produitScal(l) * 2.f;
    Vecteur r = n.mult(scal);
    r = r.moins(l).normalize();
    specf = pow(max(r.z, 0.f), spec.z);
    diff = max(0.f, n.produitScal(l));
}


TGAColor Dessin::convertirIntensite(pixel pixel){
    TGAColor color = pixel.color;
    TGAColor newColor((float)color.bgra[2] * pixel.intensite, (float)color.bgra[1]* pixel.intensite, (float)color.bgra[0]* pixel.intensite, 255);
    return newColor;
}

void Dessin::settriangle(vector<pixel> screen, TGAImage &image, float *zbuffer, Modele *modele, matrices matrices) {
    vector <pixel> box = findbox(screen[0], screen[1], screen[2]);
    Vecteur v;
    float z;
    pixel newPt;

    for (int i = box[1].x; i < box[0].x; i++){
        for (int j = box[1].y; j < box[0].y; j++) {
            newPt.x = i;
            newPt.y = j;
            v = barycentrique(newPt, screen[0], screen[1], screen[2]);
            if (isInTriangle(v)) {
                newPt.z = screen[0].z * v.x + screen[1].z * v.y + screen[2].z * v.z ;
                if (int(newPt.x + newPt.y * width) > 0 && zbuffer[int(newPt.x + newPt.y * width)] < newPt.z) {
                    zbuffer[int(newPt.x + newPt.y * width)] = newPt.z;
                    newPt.color = interpolateTriangle(v, screen[0], screen[1], screen[2], modele->diffuse, newPt);
                    newPt.colorN = interpolateTriangle(v, screen[0], screen[1], screen[2], modele->nm, newPt);
                    newPt.colorSpec = interpolateTriangle(v, screen[0], screen[1], screen[2], modele->spec, newPt);
                    if (modele->glowing)
                        newPt.colorGlow =  interpolateTriangle(v, screen[0], screen[1], screen[2], modele->glow, newPt);
                    newPt.intensite = interpolateIntensite(newPt, matrices);
                    interpolateSpec(newPt);
                    TGAColor color = convertirIntensite(newPt);
                    newPt.color = color;
                    for (int i = 0; i < 3; i++) {
                        newPt.color[i] = std::min<float>(5 + color[i] * (diff + .6 * specf + 0.30*newPt.colorGlow[i]), 255);
                    }
                    image.set(newPt.x, newPt.y, newPt.color);
                }
            }
        }
    }
}