#include "rtweekend.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include <iostream>
#include "omp.h"
#include <stdio.h>
#include <string>

#include "light.hpp"
#include "light_list.hpp"

#include "mat4x4.hpp"
#include"vec4.hpp"
#include"rectangle_xy.hpp"
#include"rectangle_xz.hpp"
#include"rectangle_yz.hpp"
#include"box.hpp"
#include"box_inside.hpp"
#include"scene.hpp"
#include"texture.hpp"
#include"image_texture.hpp"
#include "motor.hpp"

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

#define MAX_DEPTH 5
#define SAMPLES 200


int main(int argc,char *argv[]){
    //Image
    const auto aspect_ratio = 16.0/9.0; 
    const int image_width = 400;

    //Camera
    camera cam(vec3(0,0,0), vec3(0,0,-1), vec3(0,1,0), 90, aspect_ratio);

    //Moteur de rendu, écrit sur le flux sortant std::out !
    std::cerr << "Choix du moteur : Insérer le numéro" << std::endl;
    std::cerr << "1.Classique" << std::endl;
    std::cerr << "2.OpenMP" << std::endl;
    std::cerr << "3.TBB" << std::endl;
    char entry[256];
    std::cin >> entry;

    std::cerr << "Choix fichier descripteur des lumières : " << std::endl;
    char light_txt[256];
    std::cin >> light_txt;

    std::cerr << "Choix fichier descripteur des objets : " << std::endl;
    char objects_txt[256];
    std::cin >> objects_txt;

    //Lecteur de scènes !
    scene scene_des = scene();
    scene_des.object_load(objects_txt);
    scene_des.light_load(light_txt);
    

    vec3 added_light;

    if(strcmp(entry,"3")==0){
        anti_aliasing_engine_TBB engine1(SAMPLES, image_width, aspect_ratio);
        engine1.color_image(MAX_DEPTH, scene_des.light_l, scene_des.world, cam);}
    if(strcmp(entry,"2")==0){
        anti_aliasing_engine_OMP engine1(SAMPLES, image_width, aspect_ratio);
        engine1.color_image(MAX_DEPTH, scene_des.light_l, scene_des.world, cam);}
    if(strcmp(entry,"1")==0){
        anti_aliasing_engine engine1(SAMPLES, image_width, aspect_ratio);
        engine1.color_image(MAX_DEPTH, scene_des.light_l, scene_des.world, cam);}

    //Commande de compilation g++ -std=c++11 -fPIC -O3 -march=native -Wall  -o TBB.exe main2.cpp -lmpi -fopenmp -ltbb

    std::cerr << "\nDone. \n";
}
