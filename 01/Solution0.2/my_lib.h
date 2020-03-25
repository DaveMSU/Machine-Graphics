#ifndef MY_LIB
#define MY_LIB

#include "geometry.h"

class Sphere{
        Vec3f center;
        float radius;
public:
        Sphere( const Vec3f &c, const float &r ); // Иниц. сферы и радиуса.
        bool ray_intersect( const Vec3f&, const Vec3f& ); // Do 2 from 1 intersect?
};

void render();
Vec3f cast_ray( const Vec3f&, const Vec3f&, Sphere );
void render( const Sphere& );

#endif
