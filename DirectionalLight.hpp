#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "vec3.hpp"
#include "hittable.hpp"
#include "color.hpp"
#include "hittable_list.hpp"

class DirectionalLight : public light{
    public : 
        vec3 color;
        float intensity;
        vec3 direction;

        

        DirectionalLight(vec3 col,float inten,vec3 direc) : color(col),intensity(inten),direction(direc) {}

        virtual vec3 hit_light(hit_record& rec, hit_record& rec_shadow,const hittable& world) const override;

};

        vec3 DirectionalLight::hit_light(hit_record& rec, hit_record& rec_shadow,const hittable& world)const{
                vec3 value;
                
                
                float light_intensity=intensity;
                ray ray_shadow = ray(rec.p+(1e-13*rec.normal),-direction);
                //Vérification du dessin d'une ombre ou non
                if (world.hit(ray_shadow, 0.00001, infinity, rec_shadow)){
                    light_intensity = 0;
                }
                else light_intensity = intensity;
                //Puissance de la lumière dépendant de la colinéarité du rayon lumineux et de la normale de l'impact
                float light_power = std::max(0.0,dot(rec.normal,unit_vector(-direction)))*light_intensity;
                
                value = (light_power*color);

                return value;
        }
#endif