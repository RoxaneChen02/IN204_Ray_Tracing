#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "rtweekend.hpp"
#include "texture.hpp"
#include "vec3.hpp"

//Calcul du rayon réfracté
vec3 refract(double rapport_optique, vec3 direction_i, vec3 normal, double cos_theta){
    vec3 Refracted_perpendiculaire =  rapport_optique * ( unit_vector( direction_i) + cos_theta*unit_vector(normal) );
    vec3 Refracted_parallel =  -sqrt(1 - Refracted_perpendiculaire.length_squared() )* normal;
    vec3 refracted_direction = unit_vector(Refracted_parallel + Refracted_perpendiculaire);
    return refracted_direction;
}

//Calcul du rayon réfléchi
vec3 reflect(vec3 r_i, vec3 normal){
    return  unit_vector(r_i - 2*dot(r_i,normal)*normal);
}

//Calcul du coefficient de Fresnel
float Coef_Fresnel(const ray& r_in, hit_record& rec, float eta){
            float Fr; //coefficient de Fresnel : le pourcentage de lumière rélféchie
            float etai,etaf;
            if(rec.front_face){ //Rayon rentre à l'intérieur de la boule
            etai = 1;
              etaf=eta;
            }
            else{etai = eta;
              etaf=1;
                }
            double cos_theta_i = clamp(-1, 1,dot(unit_vector(r_in.direction()),-unit_vector(rec.normal))); 
            
            float sin_theta_f = etai / etaf * sqrtf(1 - cos_theta_i * cos_theta_i); 
            //std::cerr<<sin_theta_f;
            
            if (sin_theta_f >= 1) { 
                
                Fr = 1;//que de la réflextion
            } 
            else { 
                float cos_theta_f = sqrtf(1 - sin_theta_f * sin_theta_f); 
                
                float Rs = ((etaf * cos_theta_i) - (etai * cos_theta_f)) / ((etaf * cos_theta_i) + (etai * cos_theta_f)); 
                float Rp = ((etai * cos_theta_f) - (etaf * cos_theta_i)) / ((etai * cos_theta_f) + (etaf * cos_theta_i)); 
                Fr = (Rs * Rs + Rp * Rp) / 2; 
            } 
            return Fr;
        }


struct hit_record;

//On définit une classe abstraite, à partir de laquelle on va pouvoir définir les différents types de matériaux (classe fille de material)
class material {
    public:
        virtual bool scatter(
            const ray& r_in,  hit_record& rec, color& attenuation_reflected, color& attenuation_ref_dif, ray& scattered, ray& refracted, bool& bool_split_ray
        ) const = 0;

        virtual color emitted(double u, double v, const point3& p) const {
            return color(0,0,0);
        } // pour éviter d'avoir à redéfinir pour chaque classe, Si l'object n'émet rien on renvoie rien 

};


class lambertian : public material {
    public:
        lambertian(const color& a) : albedo(make_shared<one_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(
            const ray& r_in,  hit_record& rec, color& attenuation_reflected, color& attenuation_ref_dif, ray& scattered, ray& refracted, bool& bool_split_ray
        ) const override {
            bool_split_ray = false;
            auto scatter_direction = 2*(rec.normal + random_unit_vector());
            scattered = ray(rec.p, scatter_direction);
            attenuation_reflected = albedo->get_color(rec.u,rec.v,rec.p); //Gère la réflexion et la diffusion pour chacune des couleurs
            return true;
        }

    public:
        shared_ptr<texture> albedo;
};

float mix(const float &a, const float &b, const float &mix)
{
    return b * mix + a * (1 - mix);
}

class miroir : public material {
    public:
        miroir(const color& a) : albedo(make_shared<one_color>(a)) {}
        miroir(shared_ptr<texture> a) : albedo(a) {}


        virtual bool scatter(
            const ray& r_in, hit_record& rec, color& attenuation_reflected, color& attenuation_ref_dif, ray& scattered, ray& refracted, bool& bool_split_ray
        ) const override {

            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, reflected);
            
            attenuation_reflected = albedo->get_color(rec.u,rec.v,rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
};


class refraction : public material { //réfraction simple
    public:
        refraction(const double& indice_refraction, color couleur) : eta(indice_refraction), albedo(make_shared<one_color>(couleur)){}
        refraction(const double& indice_refraction, shared_ptr<texture> couleur) : eta(indice_refraction), albedo(couleur){}


        virtual bool scatter(
            const ray& r_in, hit_record& rec, color& attenuation_reflected, color& attenuation_ref_dif, ray& scattered, ray& refracted, bool& bool_split_ray
        ) const override {
            double rapport_optique;
            if(rec.front_face){ //Rayon rentre à l'intérieur de la boule
                 rapport_optique =  1/eta;
            }
            else{
               rapport_optique = eta;  //Sinon le rayon sort de la boule
            }

            
            double cos_theta = dot(-unit_vector(r_in.direction()), unit_vector(rec.normal)); //Les deux vecteurs sont dans des sens opposés
            double sin_theta =   sqrt(1 - cos_theta*cos_theta); //Racine ( 1 - cos(theta)²)
            double sin_theta_refracted = rapport_optique*sin_theta;


            if( (sin_theta_refracted > 1 )){
                //Réflexion totale
                vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
                scattered = ray(rec.p, reflected);
                attenuation_reflected =  albedo->get_color(rec.u,rec.v,rec.p);
                return true;
            }
            else{
                vec3 refracted_direction = refract(rapport_optique, r_in.direction(), rec.normal, cos_theta );
                
                scattered = ray(rec.p, refracted_direction);
                attenuation_reflected =  albedo->get_color(rec.u,rec.v,rec.p) ;//
                return true;
            }    
        }

    public:
        double eta; //La réfraction dépend de la longueur d'onde
        shared_ptr<texture> albedo;

};


class transparent : public material{  //Calcul de la réfraction physiquement, avec le coeff de Fesnel, réflechi + réfracté
    public : 
        float eta;
        shared_ptr<texture> albedo;
     transparent(const double& indice_refraction, vec3 couleur) : eta(indice_refraction), albedo(make_shared<one_color>(couleur)){}
    transparent(const double& indice_refraction, shared_ptr<texture> couleur) : eta(indice_refraction), albedo(couleur){}
    public :

        virtual bool scatter(const ray& r_in, hit_record& rec, color& color_reflected, color& color_refracted, ray& reflected, ray& refracted, bool& bool_split_ray) const override{
            double rapport_optique;
            
            // compute fresnel
           if(rec.front_face){ //Rayon rentre à l'intérieur de la boule
                rapport_optique =  1/eta; // 
            }
            else{
                rapport_optique = eta; //Sinon le rayon sort de la boule
            }

            float Fr = Coef_Fresnel(r_in, rec, eta);
            
            //std::cerr<<Fr<<"\n";
            
           
            double cos_theta = dot(-unit_vector(r_in.direction()), unit_vector(rec.normal)); //Les deux vecteurs sont dans des sens opposés
            //float Fr = mix(pow(1-cos_theta,3),1,(1+rapport_optique)/(1-rapport_optique));
            point3 bias = 1e-13*rec.normal;

            if (Fr < 1 ) { // il y a à la fois réfraction et réflexion
                //std::cerr<<"parfois";
                vec3 refracted_direction = refract(rapport_optique, unit_vector(r_in.direction()), unit_vector(rec.normal), cos_theta);
                point3 refractionRayOrig = rec.front_face ? rec.p - bias : rec.p + bias; 
                refracted = ray(refractionRayOrig,refracted_direction);
                color_refracted = (1-Fr)*albedo->get_color(rec.u,rec.v,rec.p);

                vec3 reflectionDirection = reflect(r_in.direction(), rec.normal);
                vec3 reflectionRayOrig = rec.front_face ? rec.p + bias : rec.p - bias; 
                reflected = ray(reflectionRayOrig,reflectionDirection);
        
                color_reflected = Fr*albedo->get_color(rec.u,rec.v,rec.p);
                //std::cerr<<Fr<<"\n";
                bool_split_ray  =true;
                
            } 
 
            else{
                //std::cerr<<"Fr>1";
                vec3 reflectionDirection = reflect(r_in.direction(), rec.normal);
                vec3 reflectionRayOrig = rec.front_face ? rec.p + bias : rec.p - bias; 
                reflected = ray(reflectionRayOrig,reflectionDirection);

            color_reflected = albedo->get_color(rec.u,rec.v,rec.p);
             color_refracted = color(0,0,0);
            bool_split_ray = true;}
            
 
             
            return true;
        }
};


class transparence_Schlick : public material { //Approximation de la classe Transparent sans le réel coeff de Fresnel, et possibilité de faire varier la transparence et la transmittance
    public:
        transparence_Schlick(const double& indice_refraction, color couleur, const double &refl = 0, const double &transp = 0) : eta(indice_refraction), albedo(make_shared<one_color>(couleur)), transparency(transp), reflectivity(refl) {}
        transparence_Schlick(const double& indice_refraction, shared_ptr<texture> couleur, const double &refl = 0, const double &transp = 0) : eta(indice_refraction), albedo(couleur), transparency(transp), reflectivity(refl) {}


        virtual bool scatter(
            const ray& r_in, hit_record& rec, color& attenuation_reflected, color& attenuation_ref_dif, ray& scattered, ray& refracted, bool& bool_split_ray
        ) const override {
            double rapport_optique;
            bool_split_ray = true;
            if(rec.front_face){ //Rayon rentre à l'intérieur de la boule
                rapport_optique =  1/eta; // 
            }
            else{
                rapport_optique = eta; //Sinon le rayon sort de la boule
            }

            double cos_theta = dot(-unit_vector(r_in.direction()), unit_vector(rec.normal)); //Les deux vecteurs sont dans des sens opposés
            float fresneleffect = mix(pow(1 - cos_theta, 3), 1, 0.1);

            vec3 reflected = reflect(r_in.direction(), rec.normal);
            scattered = ray(rec.p, reflected);
            attenuation_reflected = reflectivity*fresneleffect*albedo->get_color(rec.u,rec.v,rec.p);

            if(transparency >0){
                vec3 refracted_direction = refract(rapport_optique, r_in.direction(), rec.normal, cos_theta );
                
                refracted = ray(rec.p, refracted_direction);
                attenuation_ref_dif =  ( (1-fresneleffect)*transparency )*   albedo->get_color(rec.u,rec.v,rec.p) ;
            }          
            return true;
        }

    public:
        double eta; //La réfraction dépend de la longueur d'onde
        shared_ptr<texture> albedo;
        double transparency;
        double reflectivity;

};


class surface_emetteuse : public material  { //Objet émettant de la lumière, utilise la diffusion à son avantage
    public:
        surface_emetteuse(shared_ptr<texture> a, double coeff_intens) : emit(a), intensity(coeff_intens) {}
        surface_emetteuse(color c, double coeff_intens) : emit(make_shared<one_color>(c)), intensity(coeff_intens) {}

        virtual bool scatter(
            const ray& r_in, hit_record& rec, color& attenuation_reflected, color& attenuation_ref_dif, ray& scattered, ray& refracted, bool& bool_split_ray
        ) const override {
            return false; //On arrete la course de notre rayon
        }

        virtual color emitted(double u, double v, const point3& p) const override {
            return emit->get_color(u, v, p) * intensity;
        }

    public:
        shared_ptr<texture> emit;
        double intensity;
};

class granule_reflet : public material { //Effet métal avec du flou
    public:
        granule_reflet(const color& a, double flou) : albedo(make_shared<one_color>(a)), bruit(clamp(flou,0,1)) {}
        granule_reflet(shared_ptr<texture> a, double flou) : albedo(a), bruit(clamp(flou,0,1)) {}


        virtual bool scatter(
            const ray& r_in, hit_record& rec, color& attenuation_reflected, color& attenuation_ref_dif, ray& scattered, ray& refracted, bool& bool_split_ray
        ) const override {
            vec3 reflected = reflect(r_in.direction(), rec.normal);
            //On ajoute du bruit, avec du hasar, même principe que la diffusion (Ajoute du flou)
            scattered = ray(rec.p, reflected + bruit*random_unit_vector() );
            
            attenuation_reflected = albedo->get_color(rec.u,rec.v,rec.p);
            return true;
        }

    public:
        shared_ptr<texture> albedo;
        double bruit; //En réalité, le bruit correspond à quelle point la sphère diffuse (1= complète diffusion, sinon complète réflexion)
};

#endif