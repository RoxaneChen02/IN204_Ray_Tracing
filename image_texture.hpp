#ifndef IMAGE_TEXTURE_H
#define IMAGE_TEXTURE_H
#include "texture.hpp"
#include"rtweekend.hpp"

class image_texture : public texture {
public:
	image_texture() {}
	image_texture(unsigned char* pixels, int A, int B) : image(pixels), nx(A), ny(B) {}

	virtual vec3 get_color(double u, double v, const point3& p) const;

	unsigned char* image;
	int nx, ny; //la taille de l'image
};

vec3 image_texture::get_color(double u, double v, const point3& p) const {
	int i = u*nx;
	int j = (1-v)*ny ; //1-v pour que l'image soit à l'endroit !


    i = clamp(i, 0, nx-1);
    j = clamp(j,0,ny-1);

	float r = int(image[3*i+3*nx*j])/255.0;
	float g = int(image[3*i+3*nx*j+1])/255.0;
	float b = int(image[3*i+3*nx*j+2])/255.0;

	return vec3(r, g, b);
}
#endif