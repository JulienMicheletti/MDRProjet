//
// Created by julien on 12/02/19.
//

#include "Modele.h"



Modele::Modele() {}

Modele::Modele(string filename): diffuse(), nm(), spec(){
    string name = filename + "_diffuse.tga";
    string name2 = filename + "_nm.tga";
    string name3 = filename + "_spec.tga";

    diffuse.read_tga_file(name.c_str());
    diffuse.flip_vertically();
    nm.read_tga_file(name2.c_str());
    nm.flip_vertically();
    spec.read_tga_file(name3.c_str());
    spec.flip_vertically();

};
