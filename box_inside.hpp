
#ifndef BOX_inside_HPP
#define BOX_inside_HPP

#include"rectangle_xy.hpp"
#include"rectangle_xz.hpp"
#include"rectangle_yz.hpp"
#include"vec3.hpp"
#include"mat4x4.hpp"
#include"vec4.hpp"
#include"hittable_list.hpp"

class box_inside : public hittable{
    public : 
        point3 minimum;
        point3 maximum;
        mat4x4 translateMarix;
        mat4x4 scaleMatrix;
        shared_ptr<material> mat_ptr_1;
        shared_ptr<material> mat_ptr_2;

        box_inside(){}
        box_inside(mat4x4 mt, mat4x4 ms,shared_ptr<material> m,shared_ptr<material> m1){minimum = (ms*mt*vec4(-1,-1,1,1)).xyz();maximum = (ms*mt*vec4(1,1,-1,1)).xyz();mat_ptr_1 = m;mat_ptr_2 = m;}

        virtual bool hit( const ray& r, double t_min, double t_max, hit_record& rec) const override;

};

bool box_inside::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {

    hittable_list b;
    
    b.add(make_shared<rectangle_xy>(vec3(minimum.x(),minimum.y(),maximum.z()),maximum,mat_ptr_1));

    b.add(make_shared<rectangle_yz>(minimum,vec3(minimum.x(),maximum.y(),maximum.z()),mat_ptr_2));
    b.add(make_shared<rectangle_yz>(vec3(maximum.x(),minimum.y(),minimum.z()),maximum,mat_ptr_2));

    b.add(make_shared<rectangle_xz>(minimum,vec3(maximum.x(),minimum.y(),maximum.z()),mat_ptr_1));
    b.add(make_shared<rectangle_xz>(vec3(minimum.x(),maximum.y(),minimum.z()),maximum,mat_ptr_1));

    return b.hit(r,t_min,t_max,rec);

}

#endif