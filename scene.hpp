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

class scene{
    public : 

        hittable_list world;
        light_list light_l;
       
        scene(){}
        
        void light_load();
        void object_load();
};

void scene::object_load(){
    
    ifstream inFile;
    inFile.open("object_list.txt");
    if (!inFile) {
      
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }
    while(!inFile.eof()){
        char type[256];
        
        inFile >> type;
        
        if (strcmp(type,"Sphere")==0){
            
            vec3 cen = vec3();
            color col = color();
            float radius;
            char mat[256];
            char texture[256];
            char titre[256];
            float eta;
            int nx,ny,n;
            

            inFile >> cen.e[0] >> cen.e[1]>> cen.e[2]>> radius >> mat >> texture;
                    
            //std::cerr << cen.e[0] <<cen.e[1]<<cen.e[2]<< radius<< mat<< col.e[0]<< col.e[1]<< col.e[2];
            if (strcmp(mat,"lambertian") == 0) {
                if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<sphere>(cen, radius, make_shared<lambertian>(make_shared<one_color>(col))));
                }

                 if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<sphere>(cen, radius, make_shared<lambertian>(make_shared<image_texture>(image,nx,ny))));
                }
            }
            
            if (strcmp(mat,"miroir")==0){
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<sphere>(cen, radius, make_shared<miroir>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<sphere>(cen, radius, make_shared<miroir>(make_shared<image_texture>(image,nx,ny))));
                }
            }


            if (strcmp(mat,"refraction")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<sphere>(cen, radius, make_shared<refraction>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<sphere>(cen, radius, make_shared<refraction>(eta,make_shared<image_texture>(image,nx,ny))));
                }
            }

             if (strcmp(mat,"transparent")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<sphere>(cen, radius, make_shared<transparent>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<sphere>(cen, radius, make_shared<transparent>(eta,make_shared<image_texture>(image,nx,ny))));
                }
            }

            if (strcmp(mat,"surface_emetteuse")==0){
                inFile >> eta; //intensité
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<sphere>(cen, radius, make_shared<surface_emetteuse>(make_shared<one_color>(col),eta)));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<sphere>(cen, radius, make_shared<surface_emetteuse>(make_shared<image_texture>(image,nx,ny),eta)));
                }
            }

            if (strcmp(mat,"granule_reflet")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<sphere>(cen, radius, make_shared<granule_reflet>(make_shared<one_color>(col),eta)));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<sphere>(cen, radius, make_shared<granule_reflet>(make_shared<image_texture>(image,nx,ny),eta)));
                }
            }
           
           

        }

        if(strcmp(type,"Plan")==0){
            vec3 normal;
            point3 po;
            char mat[256];
            color col;
            char texture[256];
            char titre[256];
            float eta;
            int nx,ny,n;
            

            inFile >> normal.e[0]>> normal.e[1]>> normal.e[2]>> po.e[0] >> po.e[1]>> po.e[2]>> mat >> texture;

            if (strcmp(mat,"lambertian") == 0) {
                if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<plan>(po, normal, make_shared<lambertian>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<plan>(po, normal, make_shared<lambertian>(make_shared<image_texture>(image,nx,ny))));
                }
            }
            
            if (strcmp(mat,"miroir")==0){
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<plan>(po, normal, make_shared<miroir>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<plan>(po, normal, make_shared<miroir>(make_shared<image_texture>(image,nx,ny))));
                }
            }


            if (strcmp(mat,"refraction")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<plan>(po, normal, make_shared<refraction>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<plan>(po, normal, make_shared<refraction>(eta,make_shared<image_texture>(image,nx,ny))));
                }
            }

             if (strcmp(mat,"transparent")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<plan>(po, normal, make_shared<transparent>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<plan>(po, normal, make_shared<transparent>(eta,make_shared<image_texture>(image,nx,ny))));
                }
                }
            }

            if (strcmp(mat,"surface_emetteuse")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<plan>(po, normal, make_shared<surface_emetteuse>(make_shared<one_color>(col),eta)));
                }

                if(strcmp(texture,"image_texture")==0){
                    if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<plan>(po, normal, make_shared<surface_emetteuse>(make_shared<image_texture>(image,nx,ny),eta)));
                }
                }
            }

            if (strcmp(mat,"granule_reflet")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<plan>(po, normal, make_shared<granule_reflet>(make_shared<one_color>(col),eta)));
                }

                if(strcmp(texture,"image_texture")==0){
                    if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<plan>(po, normal, make_shared<granule_reflet>(make_shared<image_texture>(image,nx,ny),eta)));
                }
                }
            }
        }


        /*if(strcmp(type,"Rectangle_xy")==0){
            vec3 scalevector;
            vec3 translatevector;
            char mat[256];
            color col;
            char texture[256];
            char titre[256];
            float eta;
            int nx,ny,n;
            

            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            
            if (strcmp(mat,"lambertian") == 0) {
                if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<image_texture>(titre))));
                }
            }
            
            if (strcmp(mat,"miroir")==0){
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<image_texture>(titre))));
                }
            }


            if (strcmp(mat,"refraction")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<image_texture>(titre))));
                }
            }

             if (strcmp(mat,"transparent")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"surface_emetteuse")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"granule_reflet")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xy>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<image_texture>(titre))));
                }
            }
        }

        

if(strcmp(type,"Rectangle_yz")==0){
            vec3 scalevector;
            vec3 translatevector;
            char mat[256];
            color col;
            char texture[256];
            char titre[256];
            float eta;
            int nx,ny,n;
            

            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            
            if (strcmp(mat,"lambertian") == 0) {
                if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<image_texture>(titre))));
                }
            }
            
            if (strcmp(mat,"miroir")==0){
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<image_texture>(titre))));
                }
            }


            if (strcmp(mat,"refraction")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<image_texture>(titre))));
                }
            }

             if (strcmp(mat,"transparent")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"surface_emetteuse")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"granule_reflet")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_yz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<image_texture>(titre))));
                }
            }
        }


        

        if(strcmp(type,"Rectangle_xz")==0){
            vec3 scalevector;
            vec3 translatevector;
            char mat[256];
            color col;
            char texture[256];
            char titre[256];
            float eta;
            int nx,ny,n;
            

            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            
            if (strcmp(mat,"lambertian") == 0) {
                if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<image_texture>(titre))));
                }
            }
            
            if (strcmp(mat,"miroir")==0){
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<image_texture>(titre))));
                }
            }


            if (strcmp(mat,"refraction")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<image_texture>(titre))));
                }
            }

             if (strcmp(mat,"transparent")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"surface_emetteuse")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"granule_reflet")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<rectangle_xz>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<image_texture>(titre))));
                }
            }
        }

       if(strcmp(type,"Box")==0){
            vec3 scalevector;
            vec3 translatevector;
            char mat[256];
            color col;
            char texture[256];
            char titre[256];
            int nx,ny,n;
            float eta;
            

            inFile >> scalevector.e[0]>> scalevector.e[1]>> scalevector.e[2]>> translatevector.e[0] >> translatevector.e[1]>> translatevector.e[2]>> mat >> texture;
            
            if (strcmp(mat,"lambertian") == 0) {
                if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<lambertian>(make_shared<image_texture>(titre))));
                }
            }
            
            if (strcmp(mat,"miroir")==0){
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre>>nx>>ny>>n;
                    unsigned char* image = stbi_load(titre,&nx,&ny,&n,0);
                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<miroir>(make_shared<image_texture>(image,nx,ny))));
                }
            }


            if (strcmp(mat,"refraction")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<refraction>(eta,make_shared<image_texture>(titre))));
                }
            }

             if (strcmp(mat,"transparent")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<transparent>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"surface_emetteuse")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<surface_emetteuse>(eta,make_shared<image_texture>(titre))));
                }
            }

            if (strcmp(mat,"granule_reflet")==0){
                inFile >> eta;
               if(strcmp(texture,"one_color") == 0){
                    inFile >> col.e[0] >> col.e[1] >> col.e[2];
                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<one_color>(col))));
                }

                if(strcmp(texture,"image_texture")==0){
                    inFile>>titre;

                    world.add(make_shared<box>(ScaleMatrix(scalevector), TranslateMatrix(translatevector), make_shared<granule_reflet>(eta,make_shared<image_texture>(titre))));
                }
            }
        }


     */   
    }
    
    inFile.close();
    
}

/*void scene::light_load(){
    
    ifstream inFile;
    inFile.open("light_list.txt");
    if (!inFile) {
      
        std::cerr << "Unable to open file";
        exit(1); // terminate with error
    }
    while(!inFile.eof()){
        char type[256];
        
        inFile >> type;
        std::cerr <<type<< "  " <<strcmp(type,"SphericalLight")<<"\n" ;
       
        if (strcmp(type,"DirectionalLight")==0){
            
            vec3 direction = vec3();
            color col = color();
            int intensity;

            inFile >> direction.e[0] >> direction.e[1]>> direction.e[2]>> col.e[0]>> col.e[1]>> col.e[2]>> intensity;
            
            light_l.add(make_shared<DirectionalLight>(col, intensity, direction));

        }

        if(strcmp(type,"SphericalLight")==0){
            vec3 position;
            color col;
            int intensity;

            inFile >> position.e[0]>> position.e[1]>> position.e[2]>> col.e[0] >> col.e[1]>> col.e[2]>> intensity;

            light_l.add(make_shared<SphericalLight>(position,col, intensity));
        }
        
    }
    
    inFile.close();
    
}*/

#endif