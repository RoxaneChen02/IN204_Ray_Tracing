#ifndef MOTEUR_HPP
#define MOTEUR_HPP

#include "rtweekend.hpp"
#include "camera.hpp"
#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include <iostream>
#include "omp.h"
#include <stdio.h>
#include "vec3.hpp"
#include <vector>
#include <chrono>


#include "light.hpp"
#include "light_list.hpp"
#include "DirectionalLight.hpp"
#include "PonctualLight.hpp"

//Parallélisation tbb
#include <tbb/blocked_range2d.h>
#include <tbb/partitioner.h>
#include <tbb/tbb.h>


#include <iostream>

color ray_color(const ray& r, const hittable& world, int depth, const light_list& light_l, int max_depth, bool hit_something, vec3 added_light) {
    hit_record rec;      //represents the point of intersection 
    hit_record rec_shadow;
    vec3 light_value;

    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, 0.0000001, infinity, rec)) { //Si on touche un objet
        if(depth == max_depth){
            added_light = light_l.hit_light(rec,rec_shadow,world); //On calcule l'intensité de la lumière au premier lancer de ray issu de la caméra
        }

        //Définition des variables utiles
        hit_something = true;  //Marqueur qu'un objet a été touché
        //Deux rayons et deux attenuation/albedo, pour les deux possibles rayons
        ray rayon_reflected;
        ray rayon_ref_dif;

        color attenuation_reflected;
        color attenuation_ref_dif;

        bool bool_split_ray= false;
        

        if(rec.mat_ptr->scatter(r, rec, attenuation_reflected, attenuation_ref_dif, rayon_reflected, rayon_ref_dif, bool_split_ray)){
            if(bool_split_ray) //Dans le cas, où il y a séparation, rayon réfléchi et rayon réfracté
            
                return attenuation_reflected * ray_color(rayon_reflected, world, int((depth-1)/2), light_l, max_depth, hit_something, added_light) + attenuation_ref_dif * ray_color(rayon_ref_dif, world, int((depth-1)/2), light_l, max_depth, hit_something, added_light) ;
            else //Le rayon émis est unique
                return attenuation_reflected * ray_color(rayon_reflected, world, depth-1, light_l, max_depth, hit_something, added_light);

        }
        else{
            return added_light + rec.mat_ptr->emitted(rec.u, rec.v,rec.p);
        }
    }

    if(hit_something == true){
        //std::cerr << added_light << std::flush;
        return added_light;
    }
    else{ //COULEUR DU FOND
        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0); // Création d'un gradient en fond, la caméra s'étend en y de -1 à 1
        return (1.0-t)*unit_vector(color(1.0, 1.0, 1.0)) + t*unit_vector(color(0.5,0.7,1.0));   //color(0.5, 0.7, 1.0);
        //return color(0,0,0);
    }      
}

class moteur{
    public:
        virtual void color_image(int max_depth, light_list light_scene, hittable_list world, camera cam) const = 0;
};
    

class anti_aliasing_engine : public moteur{
    public :
        anti_aliasing_engine(const int nb_pixel, const int W, const double ratio) : nombre_de_pixels(nb_pixel), image_width(W), aspect_ratio(ratio) {}

        virtual void color_image(int max_depth, light_list light_scene, hittable_list world, camera cam) const override{
            int image_height = static_cast<int>(image_width/aspect_ratio);
            vec3 added_light;
            auto start = std::chrono::system_clock::now();
            std::cout <<"P3\n" << image_width << ' ' << image_height << "\n255\n";
            for (int j = image_height-1; j >= 0; --j){
                std::cerr << "\rScanlines remaining : " << j << ' '<< std::flush;
                for(int i = 0; i < image_width; ++i){
                    color pixel_color(0, 0, 0);
                    //Samples_per_pixels pour l'anti aliasing, explique la présence d'une troisième boucle for
                    for (int s = 0; s < nombre_de_pixels; ++s) {
                        auto u = (i + random_double()) / (image_width-1);
                        auto v = (j + random_double()) / (image_height-1);
                        ray r = cam.get_ray(u, v);
                        pixel_color += ray_color(r, world, max_depth, light_scene, max_depth, false, added_light);
                    }
                    write_color(std::cout, pixel_color, nombre_de_pixels);
                }
            }
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> deltaT = end - start;
            std::cerr << std::endl << "Calcul de l'image en " << deltaT.count() << " secondes." << std::endl;
        }
        
    public :
        int nombre_de_pixels; //pour l'anti aliasing
        int image_width;
        double aspect_ratio;
};

class anti_aliasing_engine_OMP : public moteur{
    public :
        anti_aliasing_engine_OMP(const int nb_pixel, const int W, const double ratio) : nombre_de_pixels(nb_pixel), image_width(W), aspect_ratio(ratio) {}

        virtual void color_image(int max_depth, light_list light_scene, hittable_list world, camera cam) const override{
            int image_height = static_cast<int>(image_width/aspect_ratio);
            vec3 added_light;
            auto start = std::chrono::system_clock::now();
            std::cout <<"P3\n" << image_width << ' ' << image_height << "\n255\n";
            #pragma omp parallel for schedule (dynamic)
            for (int j = image_height-1; j >= 0; --j){
                std::cerr << "\rScanlines remaining : " << j << ' '<< std::flush;
                for(int i = 0; i < image_width; ++i){
                    color pixel_color(0, 0, 0);
                    //Samples_per_pixels pour l'anti aliasing, explique la présence d'une troisième boucle for
                    for (int s = 0; s < nombre_de_pixels; ++s) {
                        auto u = (i + random_double()) / (image_width-1);
                        auto v = (j + random_double()) / (image_height-1);
                        ray r = cam.get_ray(u, v);
                        pixel_color += ray_color(r, world, max_depth, light_scene, max_depth, false, added_light);
                    }
                    write_color(std::cout, pixel_color, nombre_de_pixels);
                }
            }
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> deltaT = end - start;
            std::cerr << std::endl << "Calcul de l'image en " << deltaT.count() << " secondes." << std::endl;
        }
        
    public :
        int nombre_de_pixels; //pour l'anti aliasing
        int image_width;
        double aspect_ratio;
};

class anti_aliasing_engine_TBB : public moteur{
    public :
        anti_aliasing_engine_TBB(const int nb_pixel, const int W, const double ratio) : nombre_de_pixels(nb_pixel), image_width(W), aspect_ratio(ratio) {}

        virtual void color_image(int max_depth, light_list light_scene, hittable_list world, camera cam) const override{
            int image_height = static_cast<int>(image_width/aspect_ratio);
            vec3 added_light;
            std::cout <<"P3\n" << image_width << ' ' << image_height << "\n255\n";
            std::vector<color> image(image_width * image_height);
            constexpr const int grainSize = 896;
            constexpr const int numThreads = 4;
            auto partitioner = tbb::auto_partitioner();
            tbb::task_scheduler_init init(numThreads);
            auto start = std::chrono::system_clock::now();
            tbb::parallel_for( tbb::blocked_range2d<unsigned,unsigned>(0,image_width,grainSize,0,image_height,grainSize),
                       [&]( tbb::blocked_range2d<unsigned,unsigned> const& r)
                       {
                            for (unsigned y = r.cols().begin(); y < r.cols().end(); ++y) 
                            {
                                 std::cerr << "\rScanlines remaining : " << (image_height -1 - y) << ' '<< std::flush;
                                for (unsigned x = r.rows().begin(); x < r.rows().end(); ++x) 
                                {
                                    //std::cerr << y << std::endl;
                                    //break;
                                    color pixel_color(0, 0, 0);
                                    //Samples_per_pixels pour l'anti aliasing, explique la présence d'une troisième boucle for
                                    for (int s = 0; s < nombre_de_pixels; ++s) {
                                        auto u = (x + random_double()) / (image_width-1);
                                        auto v = (y + random_double()) / (image_height-1);
                                        ray r = cam.get_ray(u, v);
                                        pixel_color += ray_color(r, world, max_depth, light_scene, max_depth, false, added_light);
                                    }
                                    image[x + y *image_width] = pixel_color;
                                }
                            }
                        }, partitioner);
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> deltaT = end - start;
            std::cerr << std::endl << "Calcul de l'image en " << deltaT.count() << " secondes." << std::endl;
            //Réécriture de l'image dans le bon sens
            for(int j = image_height -1 ; j >= 0  ;--j){
                for(int i = 0; i < image_width; ++i){
                    write_color(std::cout,image[j*image_width + i],nombre_de_pixels);
                }
            }
        }
        
    public :
        int nombre_de_pixels; //pour l'anti aliasing
        int image_width;
        double aspect_ratio;
};



#endif