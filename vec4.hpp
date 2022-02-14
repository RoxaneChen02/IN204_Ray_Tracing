#ifndef VEC4_HPP
#define VEC4_HPP

#include <cmath>
#include <iostream>

using std::sqrt;

class vec4{
    public:
        double e[4];
    public:
        vec4() : e{0,0,0,0} {}
        vec4(double e0, double e1, double e2, double e3) : e{e0, e1, e2, e3} {}
        vec4(vec3 v, double e3) : e{v.e[0], v.e[1], v.e[2], e3} {}
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }
        double w() const {return e[3];}
        vec4 operator-() const { return vec4(-e[0], -e[1], -e[2], -e[3]); }
        vec4 operator*(const vec4 &v){ return vec4(e[0]*v.e[0],e[1]*v.e[1],e[2]*v.e[2], e[3]*v.e[3]);}
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

 // la différence % à l'opération avant

        vec4& operator+=(const vec4 &v){
            e[0] += v.e[0];
            e[1]+=v.e[1];
            e[2]+=v.e[2];
            e[3]+=v.e[3];
            return *this;
        }

        
        vec4& operator*= (const double t){
            e[0]*=t;
            e[1]*=t;
            e[2]*=t;
            e[3]*=t;
            return *this;
        }

        vec4& operator/= (const double t){
            e[0]/=t;
            e[1]/=t;
            e[2]/=t;
            e[3]/=t;
            return *this;
        }


        double length_squared4() const{
            return e[0]*e[0]+e[1]*e[1]+e[2]*e[2]+e[3]*e[3];

        }

        double length4() const {
            return sqrt(length_squared4());
        }

        vec3 xyz() const{
            return vec3(e[0],e[1],e[2]);
        }


        
};

inline std::ostream& operator << (std::ostream& out, const vec4& v ){
    out<<v.e[0]<<' '<<v.e[1]<<' '<<v.e[2]<<' '<<v.e[3];
    return out;
}
        
        
inline vec4 operator +(const vec4& v, const vec4& u){
    return vec4(v.e[0]+u.e[0],v.e[1]+u.e[1],v.e[2]+u.e[2], v.e[3]+u.e[3]);
}

inline vec4 operator -(const vec4& v, const vec4& u){
    return vec4(v.e[0]-u.e[0],v.e[1]-u.e[1],v.e[2]-u.e[2],v.e[3]-u.e[3]);
}

inline vec4 operator *( double t, const vec4& v){
    return vec4(t*v.e[0],t*v.e[1],t*v.e[2],t*v.e[3]);
}

inline vec4 operator /(  const vec4& v,double t){
    return vec4(v.e[0]*1/t,1/t*v.e[1],1/t*v.e[2],1/t*v.e[3]);
}

//produit scalaire
inline double dot4(const vec4& u, const vec4& v){
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2]+ u.e[3] * v.e[3];
}


//vecteur unitaire
inline vec4 unit_vector4(vec4 v) {
    return v / v.length4();
}



#endif
