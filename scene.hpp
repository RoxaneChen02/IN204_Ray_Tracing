#ifndef SCENE_HPP
#define SCENE_HPP

#include"hittable_list.hpp"
#include"light_list.hpp"
#include <fstream>
#include"DirectionalLight.hpp"
#include"SphericalLight.hpp"
#include"vec3.hpp"
#include"material.hpp"
#include"plan.hpp"
#include"box.hpp"
#include"hittable.hpp"
#include"mat4x4.hpp"
#include"sphere.hpp"
#include"rectangle_xy.hpp"
#include"rectangle_xz.hpp"
#include"rectangle_yz.hpp"
#include"image_texture.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

shared_ptr<texture> getTexture(ifstream& inFile, char* texture,color& col,char* titre, int& nx, int& ny, int& n){

    if(strcmp(texture,"one_color") == 0){
        
        inFile >> col.e[0] >> col.e[1] >> col.e[2];
        
        return make_shared<one_color>(col);
    }

    if(strcmp(texture,"image_texture")==0){
        inFile >>titre>>nx>>ny>>n;
        unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
        
        return make_shared<image_texture>(image,nx,ny);
        }

    else {std::cerr << "erreur unexistant texture \n"; return make_shared<one_color>(color(0,0,0)); }
    
}


shared_ptr<material> getMaterial(ifstream& inFile, char* mat, float& param_mat ,char* texture,color& col,char* titre, int& nx, int& ny, int& n){
    if (strcmp(mat,"lambertian") == 0) {
        auto text = getTexture(inFile, texture ,col,titre,nx,ny, n);
        return make_shared<lambertian>(text);
        }
            
    if (strcmp(mat,"miroir")==0){
        auto text = getTexture(inFile, texture ,col,titre,nx,ny, n);
        return make_shared<miroir>(text);
    }


    if (strcmp(mat,"refraction")==0){
        inFile >>param_mat; // indice de réfraction de l'objet
        auto text = getTexture(inFile, texture ,col,titre,nx,ny, n);
        return make_shared<refraction>(param_mat,text);
    }

    if (strcmp(mat,"transparent")==0){
        inFile >>param_mat; // indice de réfraction de l'objet
        auto text = getTexture(inFile, texture ,col,titre,nx,ny, n);
        return make_shared<transparent>(param_mat,text);
    }

    if (strcmp(mat,"surface_emetteuse")==0){
        inFile >> param_mat; //intensité
        auto text = getTexture(inFile, texture ,col,titre,nx,ny, n);    
        return make_shared<surface_emetteuse>(text,param_mat);
    }

    if (strcmp(mat,"granule_reflet")==0){
        inFile >> param_mat; //intensité
        auto text = getTexture(inFile, texture ,col,titre,nx,ny, n); 
        return make_shared<granule_reflet>(text,param_mat);
    }
}


class scene{
    public : 

        hittable_list world;
        light_list light_l;
       
        scene(){}
        
        void light_load(char* light_txt);
        void object_load(char* objects_txt);
};

void scene::object_load(char* objects_txt){
    
    ifstream inFile;
    inFile.open(objects_txt);
    if (!inFile) {
      
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }
    while(!inFile.eof()){
        char type[256];
        vec3 cen = vec3();
        color col = color();
        float radius;
        char mat[256];
        char texture[256];
        char titre[256];
        float eta;
        int nx,ny,n;
        vec3 normal;
        point3 po;
        vec3 scalevector;
        vec3 translatevector;

        inFile >> type;
        
        if (strcmp(type,"Sphere")==0){
            
            inFile >> cen.e[0] >> cen.e[1]>> cen.e[2]>> radius >> mat >> texture;


            auto materiel = getMaterial(inFile, mat, eta,texture,col,titre,nx, ny,n);
            world.add(make_shared<sphere>(cen, radius, materiel));

        }

        if(strcmp(type,"Plan")==0){
        

            inFile >> normal.e[0]>> normal.e[1]>> normal.e[2]>> po.e[0] >> po.e[1]>> po.e[2]>> mat >> texture;
            auto materiel = getMaterial(inFile, mat, eta,texture,col,titre,nx, ny,n);
            world.add(make_shared<plan>(po, normal, materiel));}



        if(strcmp(type,"Rectangle_xy")==0){

            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            auto materiel = getMaterial(inFile, mat, eta,texture,col,titre,nx, ny,n);
            world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), materiel));}

        

        if(strcmp(type,"Rectangle_yz")==0){


            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            auto materiel = getMaterial(inFile, mat, eta,texture,col,titre,nx, ny,n);
            world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), materiel));}


        if(strcmp(type,"Rectangle_xz")==0){

            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            auto materiel = getMaterial(inFile, mat, eta,texture,col,titre,nx, ny,n);
            world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), materiel));}

       if(strcmp(type,"Box")==0){

            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            auto materiel = getMaterial(inFile, mat, eta,texture,col,titre,nx, ny,n);
            world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), materiel));}
    }
          
    
}

void scene::light_load(char* list_txt){
    
    ifstream inFile;
    inFile.open(list_txt);
    if (!inFile) {
      
        std::cerr << "Unable to open file";
        exit(1); 
    }
    while(!inFile.eof()){
        char type[256];
        vec3 direction = vec3();
        color col = color();
        int intensity;
        vec3 position;

        inFile >> type;
        if (strcmp(type,"DirectionalLight")==0){
            
            

            inFile >> direction.e[0] >> direction.e[1]>> direction.e[2]>> col.e[0]>> col.e[1]>> col.e[2]>> intensity;
            
            light_l.add(make_shared<DirectionalLight>(col, intensity, direction));

        }

        if(strcmp(type,"SphericalLight")==0){
            

            inFile >> position.e[0]>> position.e[1]>> position.e[2]>> col.e[0] >> col.e[1]>> col.e[2]>> intensity;

            light_l.add(make_shared<SphericalLight>(position,col, intensity));
        }
        
    }
    
    inFile.close();
    
}

#endif