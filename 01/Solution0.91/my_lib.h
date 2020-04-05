#ifndef MY_LIB
#define MY_LIB

#define pi 3.141592654
#define background Vec3f( 0.2, 0.7, 0.8 )
#define MAX_DEPTH 5
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
	Material(){}
	float backg_coef;     // Коэффициент фонового    освещения.
	float spec_coef;      // Коэффициент зеркального освещения.
	float diff_coef;      // Коэффициент диффузного  освещения.
	bool mirror;
	Vec3f color;
	float refr_coef;      // Коэфиент преломления среды.(внутри материала)
	Material operator*( const float number ) const;
	float operator*( const Material& right ) const;
};


// Убрать все переменные в private!!!!!
class Object{
	
	Material material;
public:
	Object(){}
	virtual float ray_intersect( const Vec3f& orig, const Vec3f& dir ) const = 0;
	virtual Material get_material( const Vec3f& point ) const = 0;
	virtual Vec3f get_normal( const Vec3f& point ) const = 0;
};


class Chessdesk: public Object{
	
	Material first_material;
	Material second_material;
	Vec3f normal;
	float bias;
	float call_size;
	Vec3f O;
	Vec3f c1; 
	Vec3f c2;
public:
	Chessdesk( const Vec3f& n,
		   const float& b, 
		   const float& size,
		   const Material& m1,
		   const Material& m2 );

	float ray_intersect( const Vec3f& orig, const Vec3f& dir ) const;
	Material get_material( const Vec3f& point ) const;
	Vec3f get_normal( const Vec3f& point ) const;

	~Chessdesk(){}
};


class Sphere: public Object{
	
	Material material;
	Vec3f center;
	float radius;
public:
	// Инициализируем центр сферы, ее радиус и материал.
        Sphere( const Vec3f &c, const float &r, const Material& m );
	Material get_material( const Vec3f& point ) const; // Попробовать убрать параметры!!!!!!
	Vec3f get_normal( const Vec3f& point ) const;
	float get_radius() const;
	
       	// Пересекает ли сферу луч, исходящий из orig, по направлению dir, если true - расстояние до нее, иначе - -1.
        float ray_intersect( const Vec3f& orig, const Vec3f& dir ) const;

	~Sphere(){}
};

// Для нахождения OY, по OX и normal в Chessdesk.get_material().
Vec3f vector_product( const Vec3f& v1, const Vec3f& v2 );


// Пересекает ли наш луч какой либо из объектов?(true/false) Если да, то присвоить this_material материал этого объекта.
//bool scene_intersect( const Vec3f&, Vec3f&, std::vector <Sphere>, Material& this_material ); // Почему работает?!?
Vec3f cast_ray( const Vec3f& orig, const Vec3f& dir, const std::vector <Sphere>& spheres, const std::vector <Light>& lights, size_t am_rec );


//Vec3f cast_ray( const Vec3f&, const Vec3f&, std::vector <Sphere> );			       // Почему работает?!?
void render( const std::vector<Object*>&, 
	     const std::vector<Light>&,
	     const std::string outFilePath );

#endif
