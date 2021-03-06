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
	Vec3f  operator+( const Vec3f& right ) const;
	Vec3f  operator+( const float number ) const;
	Vec3f  operator-( const Vec3f& right ) const;
	Vec3f  operator-( const float number ) const;
	float  operator*( const Vec3f& right ) const;
	Vec3f  operator*( const float number ) const;
	Vec3f  operator/( const float number ) const;
	Vec3f& operator*=( Vec3f& left, const float number );
	void normalize();
	Vec3f normalize();
	~Vec3f();
};

#endif
