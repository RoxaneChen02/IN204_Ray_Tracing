#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.hpp"

class texture {
    public:
        virtual color get_color(double u, double v, const point3& p) const = 0;
};

class one_color : public texture {
    public:
        one_color() {}
        one_color(color c) : color_value(c) {}

        virtual color get_color(double u, double v, const vec3& p) const override {
            return color_value;
        }

    public:
        color color_value;
};

#endif