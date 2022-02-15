#ifndef SPHERICALLIGHT_HPP
#define SPHERICALLIGHT_HPP


#include "vec3.hpp"
#include "hittable.hpp"
#include "color.hpp"



class SphericalLight : public light {
    public : 
        point3 position;
        vec3 color;
        float intensity;
        double radius;
        //char type = 'S';
        SphericalLight(point3 p,vec3 c, float inten, double rayon) : position(p),color(c),intensity(inten), radius(rayon) {}
        virtual vec3 hit_light(
            hit_record& rec, hit_record& rec_shadow,const hittable& world) const override;

};

        vec3 SphericalLight::hit_light(hit_record& rec, hit_record& rec_shadow, const hittable& world) const {
                vec3 value;
                float light_intensity = intensity;
                ray ray_shadow = ray(rec.p+(1e-13*rec.normal),unit_vector(position-rec.p));
                //Calcul de l'ombre
                if (world.hit(ray_shadow, 0.0001, infinity, rec_shadow) )
                {
                    if ((rec_shadow.p-rec.p).length()<(rec.p-position).length()){      
                        light_intensity = 0;
                    }
                }

                vec3 ray_direction = unit_vector(position-rec.p);
                //Puissance de la lumière dépenante de la colinéarité du vecteur lumineux et de la normale de l'impact
                //Dépedance de la puissance avec la distance parcouru par le rayon lumineux (Atténuation sphérique)
                light_intensity = std::max(0.0, dot(rec.normal,-unit_vector(rec.p-position)))* light_intensity / (4*pi*(position + ray_direction * radius -rec.p).length_squared());
                value = (light_intensity *color);
                return value;
        }

#endif