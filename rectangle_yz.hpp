#ifndef RECTANGLE_YZ_HPP
#define RECTANGLE_YZ_HPP

#include "hittable.hpp"
#include "vec3.hpp"
#include "vec4.hpp"


class rectangle_yz : public hittable{
    public:
        rectangle_yz() {}
        rectangle_yz(const point3& a, const point3& b,  shared_ptr<material> m) { minimum = a; maximum = b;mat_ptr =m;}
        rectangle_yz(mat4x4 mat_t, mat4x4 mat_s,  shared_ptr<material> m) { minimum =(mat_t* mat_s*vec4(0,-1,1,1)).xyz() ; maximum = (mat_t* mat_s*vec4(0,1,-1,1)).xyz();mat_ptr = m;}

        virtual bool hit( const ray& r, double t_min, double t_max, hit_record& rec) const override;

        point3 minimum;
        point3 maximum;
        
        shared_ptr<material> mat_ptr;

};

bool rectangle_yz::hit(const ray& r, double t_min, double t_max,hit_record& rec) const {

            

float t = (minimum.x() - r.origin().x()) / r.direction().x();
vec3 hitPoint = r.at(t);

bool hit = hitPoint.y() <= maximum.y() && hitPoint.y() >= minimum.y() &&
           hitPoint.z() >= maximum.z() && hitPoint.z() <= minimum.z();

if(t_min>t||t>t_max) {
   //std::cerr<<"t<10";
    return false;}

if (hit ){
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 n = vec3(1,0,0);
            rec.set_face_normal(r, n); //détermine le sens de la normal 
            rec.mat_ptr = mat_ptr;
            if(rec.p.x()>0){
            rec.u = 1-(rec.p.z()-minimum.z())/(maximum.z()-minimum.z());
            rec.v = (rec.p.y()-minimum.y())/(maximum.y()-minimum.y());}
            if(rec.p.x()<0){
            rec.u = (rec.p.z()-minimum.z())/(maximum.z()-minimum.z());
            rec.v = (rec.p.y()-minimum.y())/(maximum.y()-minimum.y());}
            return true;
        }


return false;}


#endif