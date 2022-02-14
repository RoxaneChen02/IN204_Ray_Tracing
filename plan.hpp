#ifndef PLAN_HPP
#define PLAN_HPP

#include "hittable.hpp"
#include "vec3.hpp"

class plan : public hittable {
    public:
        plan() {}
        plan(point3 p_0, vec3 n, shared_ptr<material> m) : point(p_0), normal(n), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;



    public:
        point3 point; //un point du plan
        vec3 normal ; //normal au plan normalisée 
       shared_ptr<material> mat_ptr;
};

bool plan::hit(const ray& r, double t_min, double t_max, hit_record& rec) const{

    float inter;

    if(dot(normal,r.direction())>1e-6){ //si le dénominateur est > 0, alors il y a une intersection qui est inter 
        inter = dot((point-r.origin()),normal)/dot(normal,r.direction());
        if ( t_min < inter && inter < t_max){
            rec.t = inter;
            rec.p = r.at(rec.t);
            vec3 normalise = normal / normal.length();  
            rec.set_face_normal(r, normalise); //détermine le sens de la normal 
            rec.mat_ptr = mat_ptr;
            return true;
        }
        
    }

    return false;
    
}

#endif