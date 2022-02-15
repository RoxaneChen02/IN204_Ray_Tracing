#ifndef RECTANGLE_XZ_HPP
#define RECTANGLE_XZ_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "vec4.hpp"


class rectangle_xz : public hittable{
    public:
        rectangle_xz() {}
        rectangle_xz(const point3& a, const point3& b, shared_ptr<material> m) { minimum = a; maximum = b;mat_ptr =m;}
        rectangle_xz(mat4x4 mat_t, mat4x4 mat_s,  shared_ptr<material> m) { minimum =(mat_t* mat_s*vec4(-1,0,1,1)).xyz() ; maximum = (mat_t* mat_s*vec4(1,0,-1,1)).xyz();mat_ptr= m;}

        virtual bool hit( const ray& r, double t_min, double t_max, hit_record& rec) const override;

        point3 minimum;
        point3 maximum;
       
        shared_ptr<material> mat_ptr;
};

bool rectangle_xz::hit(const ray& r, double t_min, double t_max,hit_record& rec) const {

            
double t = (minimum.y() - r.origin().y()) / r.direction().y();

vec3 hitPoint = r.origin() + t* r.direction() ;

bool hit = hitPoint.x() <= maximum.x() && hitPoint.x() >= minimum.x() &&
           hitPoint.z() >= maximum.z() && hitPoint.z() <= minimum.z();

if(t_min>t||t>t_max) {
  
    return false;}
 

if (hit ){ 
 
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 n = vec3(0,1,0);
            rec.set_face_normal(r, n);  
            rec.mat_ptr = mat_ptr;
            rec.u = (rec.p.x()-minimum.x())/(maximum.x()-minimum.x());
            rec.v = (rec.p.z()-minimum.z())/(maximum.z()-minimum.z());
            return true;
        }


return false;}


#endif