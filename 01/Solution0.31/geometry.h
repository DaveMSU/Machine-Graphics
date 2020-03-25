#ifndef GEOM
#define GEOM

#include <vector>

class Vec3f{
	float rgb[3];
public:
	Vec3f();
	Vec3f( float red, float green, float blue );
	std::vector <float> get_rgb();
	float& operator[]( int index );
	Vec3f operator+( const Vec3f& right );
	Vec3f operator+( const float number );
	Vec3f operator-( const Vec3f& right );
	Vec3f operator-( const float number );
	float operator*( const Vec3f& right ) const;
	Vec3f operator*( const float number );
	Vec3f operator/( const float number );
	void normalize();
	~Vec3f();
};

#endif
