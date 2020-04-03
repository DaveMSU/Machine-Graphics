#ifndef MY_LIB
#define MY_LIB

#define pi 3.141592654
#define background Vec3f( 0.2, 0.7, 0.8 )
#define MAX_DEPTH 2
#include "geometry.h"

class Light{
public:
	Light( const Vec3f& p, const float& i ); 
	Vec3f position;	 // Центр источника света.
	float intensity; // Интенсивность источника.
};

class Material{
public:	
	Material( const Vec3f& color, 
		  const float bg, 
		  const float df, 
		  const float sc, 
		  const bool flag,
		  const float rf );
	Material();
	float backg_coef;     // Коэффициент фонового    освещения.
	float spec_coef;      // Коэффициент зеркального освещения.
	float diff_coef;      // Коэффициент диффузного  освещения.
	bool mirror;
	Vec3f color;
	float refr_coef;      // Коэфиент преломления среды.(внутри материала)
	Material operator*( const float number ) const;
	float operator*( const Material& right ) const;
};

class Object{
public:
	Vec3f normal;
	float bias;
	float call_size;

	Vec3f center;
	float radius;

	Material material;
	float ray_intersect( const Vec3f& orig, const Vec3f& dir ) const;
};

class Chessdesk: public Object{
public:
	Vec3f normal;
	float bias;
	float call_size;
	Material material;

	Chessdesk( const Vec3f& n,
		   const float& b, 
		   const float& size,
		   const Material& m );

	float ray_intersect( const Vec3f& orig, const Vec3f& dir ) const;
};

class Sphere: public Object{
public:
	Vec3f center;
        float radius;      
	Material material; 

	// Инициализируем центр сферы, ее радиус и материал.
        Sphere( const Vec3f &c, const float &r, const Material& m );

	
       	// Пересекает ли сферу луч, исходящий из orig, по направлению dir, если true - расстояние до нее, иначе - -1.
        float ray_intersect( const Vec3f& orig, const Vec3f& dir ) const;
};

// Пересекает ли наш луч какой либо из объектов?(true/false) Если да, то присвоить this_material материал этого объекта.
//bool scene_intersect( const Vec3f&, Vec3f&, std::vector <Sphere>, Material& this_material ); // Почему работает?!?
Vec3f cast_ray( const Vec3f& orig, const Vec3f& dir, const std::vector <Sphere>& spheres, const std::vector <Light>& lights, size_t am_rec );


//Vec3f cast_ray( const Vec3f&, const Vec3f&, std::vector <Sphere> );			       // Почему работает?!?
void render( const std::vector<Sphere>&, const std::vector<Light>& );

#endif
