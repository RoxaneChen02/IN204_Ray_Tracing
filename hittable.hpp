#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "ray.hpp"
#include "rtweekend.hpp"

class material;

struct hit_record {
    point3 p; //Point d'incidence
    vec3 normal;
    shared_ptr<material> mat_ptr; //Indique le type de diffusion, selon le matériau
    double t;
    double u,v;
    bool front_face;
    //For ray_color   
    //ray original_ray; 

    inline void set_face_normal(const ray& r, const vec3& outward_normal) { //Toujours le rayon qui vient de l'extérieur
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};


//Classe abstraite qui permet de définir toutes les classes filles comme hittable_list ou spheres...
class hittable {
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif