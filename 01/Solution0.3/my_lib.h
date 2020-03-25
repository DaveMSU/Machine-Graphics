#ifndef MY_LIB
#define MY_LIB

#include "geometry.h"

class Material{
public:	
	Material( const Vec3f& color );
	Material();
	Vec3f diffuse_color;
};

class Sphere{
public:
	Vec3f center;
        float radius;
	Material material;

        Sphere( const Vec3f &c, const float &r, const Material& m ); // Иниц. центра, радиуса и материала.
        bool ray_intersect( const Vec3f&, const Vec3f& ); // Do 2 from 1 intersect?
};

void render();
Vec3f cast_ray( const Vec3f&, const Vec3f&, Sphere );
void render( const std::vector<Sphere>& );

#endif
