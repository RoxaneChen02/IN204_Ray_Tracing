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
#include "DirectionalLight.hpp"
#include "SphericalLight.hpp"
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




int main(int argc,char *argv[]){
    //Image
    const auto aspect_ratio = 16.0/9.0; 
    const int image_width = 400;
    //const int image_height = static_cast<int>(image_width/aspect_ratio);
    const int samples_per_pixel = 400;

    //Nombre d'itération de rayons diffusés
    const int max_depth = 10;

     // Liste d'objets présents sur la scène et leur description
    //hittable_list world;
    //Description du type de réflexion ou de diffusion
    //auto material_sol = make_shared<lambertian>(color(0.1, 0.25, 0.5));
    //auto material_centre = make_shared<refraction>(1,unit_vector(color(1,1,1)));
    //auto material_gauche   = make_shared<metal>(color(0.8, 0.8, 0.8));
    //auto material_droite = make_shared<metal>(color(0.8, 0.6, 0.2));
    //auto material_centre = make_shared<lambertian>(unit_vector(color(1,1,1)));
    //auto material_gauche   = make_shared<lambertian>(color(1,1,1));
    //auto texture_droite = one_color(color(1,0,0));
    
    

    //Ajout d'objets sur la scène
    //world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, make_shared<lambertian>(color(0,0,0))));
    //world.add(make_shared<sphere>(point3( 0,    0.5, -2.0),   1, make_shared<transparent>(1.2,color(1,1,1))));
    //world.add(make_shared<sphere>(point3( 0,    0.5, -5.0),   0.7, make_shared<lambertian>(color(0,0,1))));
    //world.add(make_shared<sphere>(point3( 2.25,    0.5, -1),   1, make_shared<lambertian>(color(0,1,0))));
    //world.add(make_shared<sphere>(point3(0, -0.5 , -2),   0.5, make_shared<surface_emetteuse>(color(1,1,1),10)));
    //world.add(make_shared<sphere>(point3( 0,    0.5, -4.0),   1, make_shared<lambertian>(color(0,1,0))));
    //world.add(make_shared<sphere>(point3( -1,    0, -2.0),   0.5, make_shared<lambertian>(color(1,1,0))));
    //int nx = 1000;
    //int ny = 500;
    //int nn = 3;
    //unsigned char * earth = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);

    //world.add(make_shared<sphere>(point3(0,    0.0, -1),   0.5, make_shared<lambertian>(make_shared<image_texture>(earth,nx,ny))));
    //world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_centre));
     //mat4x4 scaleMatrice = ScaleMatrix(vec3(1,1,1));
     //mat4x4 translateMatrice = TranslateMatrix(vec3(0,-1,-1));
     //mat4x4 translateMatrice1 = TranslateMatrix(vec3(0,-1,-4));
     //mat4x4 scaleMatrice1 = ScaleMatrix(vec3(1,1,1));
     //world.add(make_shared<rectangle_xz>(ScaleMatrix(vec3(1,1,1)),TranslateMatrix(vec3(0,1,-1)),make_shared<surface_emetteuse>(color(1,1,1),4)));
    //world.add(make_shared<rectangle_xy>(scaleMatrice,translateMatrice,make_shared<transparent>(1,vec3(1,1,1))));
    
    //world.add(make_shared<rectangle_yz>(scaleMatrice,translateMatrice,make_shared<lambertian>(color(1,1,1))));
    //world.add(make_shared<box_inside>(scaleMatrice1,translateMatrice1,make_shared<lambertian>(color(0,1,0)),make_shared<lambertian>(color(1,0,0))));

    //Light
    light_list light_scene;
    light_scene.add(make_shared<DirectionalLight>(unit_vector(color(0.5,0.7,1.0)), 1, vec3(0,0,-1)));
    light_scene.add(make_shared<DirectionalLight>(unit_vector(color(0.5,0.7,1.0)), 1, vec3(-1,-1,0)));
    //light_scene.add(make_shared<DirectionalLight>(unit_vector(color(1,1,1)), 1, vec3(1,-1,0)));
    //light_scene.add(make_shared<DirectionalLight>(unit_vector(color(1,0,0)), 2, vec3(1,-1,0)));
    //light_scene.add(make_shared<DirectionalLight>(unit_vector(color(0,0,0.5)), 2, vec3(-1,-1,0)));
    //light_scene.add(make_shared<DirectionalLight>(unit_vector(color(0,0,1)), 2, vec3(-1,-1,0)));
    //light_scene.add(make_shared<DirectionalLight>(unit_vector(color(1,1,1)), 15, vec3(1,0,0)));
    //light_scene.add(make_shared<SphericalLight>(point3(1,1,-1),unit_vector(color(0.5,0.7,0)),1));
    //light_scene.add(make_shared<SphericalLight>(point3(0,1,0), unit_vector(color(1,1,1)), 1, 0.5));


    scene scene_des = scene();
    scene_des.object_load();
    //scene_des.light_load();

    vec3 added_light;

    //Camera
    camera cam;

    //Moteur de rendu, écrit sur le flux sortant std::out !
    //anti_aliasing_engine engine1(samples_per_pixel, image_width, aspect_ratio);
    //Commande de compilation g++ -std=c++11 -fPIC -O3 -march=native -Wall  -o TBB.exe main2.cpp -lmpi -fopenmp -ltbb
    //anti_aliasing_engine_TBB engine1(samples_per_pixel, image_width, aspect_ratio);
    std::cerr << "Choix du moteur : Insérer le numéro" << std::endl;
    std::cerr << "1.Classique" << std::endl;
    std::cerr << "2.OpenMP" << std::endl;
    std::cerr << "3.TBB" << std::endl;
    char entry[256];
    std::cin >> entry;
    if(strcmp(entry,"3")==0){
        anti_aliasing_engine_TBB engine1(samples_per_pixel, image_width, aspect_ratio);
        //engine1.color_image(max_depth, scene_des.light_l, world, cam);
        engine1.color_image(max_depth, light_scene, scene_des.world, cam);}
    if(strcmp(entry,"2")==0){
        anti_aliasing_engine_OMP engine1(samples_per_pixel, image_width, aspect_ratio);
        //engine1.color_image(max_depth, scene_des.light_l, world, cam);
        engine1.color_image(max_depth, light_scene, scene_des.world, cam);}
    if(strcmp(entry,"1")==0){
        anti_aliasing_engine engine1(samples_per_pixel, image_width, aspect_ratio);
        //engine1.color_image(max_depth, scene_des.light_l, world, cam);
        engine1.color_image(max_depth, light_scene, scene_des.world, cam);}

    

    std::cerr << "\nDone. \n";
}