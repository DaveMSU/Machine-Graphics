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

	// Инициализируем центр сферы, ее радиус и материал.
        Sphere( const Vec3f &c, const float &r, const Material& m );
	
       	// Пересекает ли сферу луч, исходящий из orig, по направлению dir, если true - расстояние до нее, иначе - -1.
        float ray_intersect( const Vec3f& orig, const Vec3f& dir );
};

// Пересекает ли наш луч какой либо из объектов?(true/false) Если да, то присвоить this_material материал этого объекта.
//bool scene_intersect( const Vec3f&, Vec3f&, std::vector <Sphere>, Material& this_material ); // Почему работает?!?
//Vec3f cast_ray( const Vec3f&, const Vec3f&, std::vector <Sphere> );                          // Почему работает?!?
void render( const std::vector<Sphere>& );

#endif
