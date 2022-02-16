#ifndef RECTANGLE_XY_HPP
#define RECTANGLE_XY_HPP

#include "hittable.hpp"
#include "vec3.hpp"


class rectangle_xy : public hittable{
    public:
        rectangle_xy(const point3& a, const point3& b, shared_ptr<material> m) { minimum = a; maximum = b;mat_ptr =m;}
        rectangle_xy(mat4x4 mat_t, mat4x4 mat_s,  shared_ptr<material> m) { minimum =(mat_s*(mat_t*vec4(-1,-1,0,1))).xyz() ; maximum = (mat_s*(mat_t* vec4(1,1,0,1))).xyz();mat_ptr= m;}

        virtual bool hit( const ray& r, double t_min, double t_max, hit_record& rec) const override;

        point3 minimum;
        point3 maximum;
        shared_ptr<material> mat_ptr;
};

bool rectangle_xy::hit(const ray& r, double t_min, double t_max,hit_record& rec) const {

            

double t = (minimum.e[2] - r.origin().e[2]) / r.direction().e[2];
vec3 hitPoint = r.at(t);

bool hit = hitPoint.x() <= maximum.x() && hitPoint.x() >= minimum.x() &&
           hitPoint.y() <= maximum.y() && hitPoint.y() >= minimum.y();


if(t_min>t||t>t_max) return false;

if (hit ){
            rec.t = t;
            rec.p = r.at(rec.t);
            vec3 n = vec3(0,0,1);
            rec.set_face_normal(r, n); //détermine le sens de la normal 
            rec.mat_ptr = mat_ptr;

            //calcul de u et v 
            rec.u = (rec.p.x()-minimum.x())/(maximum.x()-minimum.x());
            rec.v = (rec.p.y()-minimum.y())/(maximum.y()-minimum.y());
            return true;
        }


return false;}


#endif