#ifndef ONE_COLOR_HPP
#define ONE_COLOR_HPP

class one_color:public texture{

    public:
    one_color() {}
	one_color(color c) : col(c) {}

	virtual color get_color(float u,float v, point3& p) const override;
  
	color col;
};

color one_color::get_color(float u, float v, point3& p) const {
    return col;
}
#endif