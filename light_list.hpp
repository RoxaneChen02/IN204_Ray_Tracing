#ifndef LIGHT_LIST_HPP
#define LIGHT_LIST_HPP

#include "light.hpp"
#include "vec3.hpp"
#include "hittable.hpp"
#include "color.hpp"


class light_list : public light { 
    public:
        light_list() {}
        light_list(shared_ptr<light > object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<light> object) { objects.push_back(object); }

    public:
        std::vector<shared_ptr<light>> objects;
        virtual vec3 hit_light(hit_record& rec,hit_record& rec_shadow,const hittable& world) const override;
};

vec3 light_list::hit_light(hit_record& rec,hit_record& rec_shadow,const hittable& world) const{
    vec3 value = color(0,0,0);
    for (auto& light : objects) { 
            value += light->hit_light(rec,rec_shadow,world);}
    return value;
}

#endif