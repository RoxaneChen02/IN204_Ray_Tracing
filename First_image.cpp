
#include"vec3.hpp"
#include"color.hpp"
#include"ray.hpp"
#include <iostream>
#include "rtweekend.hpp"

#include "color.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "camera.hpp"
#include"plan.hpp"
#include"disk.hpp"
#include"box.hpp"
#include "light.hpp"
#include "light_list.hpp"
#include "DirectionalLight.hpp"
#include "SphericalLight.hpp"


color ray_color(const ray& r, const hittable& world, const light_list& light_l) {
    hit_record rec;
    hit_record rec_shadow;
    if (world.hit(r, 0, infinity, rec)) { //objet 
        
        color value = light_l.hit_light(rec,rec_shadow,world) ;
        return value;

        //return 0.5*(std::max(0.0,dot(rec.normal,-r.direction()))*color(1,1,1)); //coloration selon la normal
    }
    //le fond
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
}


int main() {

    // Image
   const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
     const int samples_per_pixel = 100;

// World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0.5,0,-1), 0.5,color(0,1,0)));
    world.add(make_shared<sphere>(point3(-0.5,0,-0.9), 0.5,color(1,0,0)));
    world.add(make_shared<plan>(point3(2,-1,-1),vec3(0,-1,0),color(1,1,1)));
    //world.add(make_shared<disk>(point3(1.4,0.6,-1),vec3(0,4,1),1));
    world.add(make_shared<box>(point3(-0.6,-0.6,-0.6),point3(0.6,0.6,-3)));

//Light
    light_list light_scene;
    light_scene.add(make_shared<DirectionalLight>(color(0.2,0.2,0.2), 15, vec3(-2,-1,0)));
    light_scene.add(make_shared<DirectionalLight>(color(0.5,0.5,0.5), 15, vec3(0.5,-1,0)));
    light_scene.add(make_shared<SphericalLight>(point3(0.5,0.5,-0.5),color(0.5,0.5,0.5),15));
    light_scene.add(make_shared<SphericalLight>(point3(0,-1,-1),color(0.5,0.7,0.5),15));
    // Camera

    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world,light_scene);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);

        }
    }
    std::cerr << "\nDone.\n";
    return 0;
}