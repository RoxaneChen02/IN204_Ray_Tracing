#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "rtweekend.hpp"

class camera {
    public:
        camera(vec3 position_camera, vec3 position_objectif, vec3 updirection_camera ,double diametre_apparent, double aspect_ratio) {
            auto alpha = diametre_apparent* pi/360;
            auto h = tan(alpha/2);
            auto viewport_height = 2.0*h;
            auto viewport_width = aspect_ratio * viewport_height;

            auto vecteur_focal = unit_vector(position_camera - position_objectif); //Ici unitaire
            auto u = unit_vector(cross(updirection_camera, vecteur_focal)); //Appartient au plan, produit vector
            auto v = cross(vecteur_focal, u); //Construction de la base de l'observateur

            origin = position_camera;
            horizontal = viewport_width * u;
            vertical = viewport_height * v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - vecteur_focal;
        }

        ray get_ray(double x, double y) const {
            return ray(origin, lower_left_corner + x*horizontal + y*vertical - origin);
        }

    private:
        point3 origin;
        point3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;
};


#endif