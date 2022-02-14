#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "vec3.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "color.hpp"

class light {
    public : 
        virtual vec3 hit_light(hit_record& rec,hit_record& rec_shadow,const hittable& world) const = 0;
};


#endif