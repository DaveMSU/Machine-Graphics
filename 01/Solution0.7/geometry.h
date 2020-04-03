#ifndef GEOM
#define GEOM

#include <vector>

class Vec3f{
        float rgb[3];
public:
        Vec3f();
        Vec3f( float red, float green, float blue );
        std::vector <float> get_rgb();
        float& operator[]( size_t index );
        const float& operator[]( size_t index ) const;
        Vec3f  operator+( const Vec3f& right ) const;
        Vec3f  operator+( const float number ) const;
        Vec3f  operator-( const Vec3f& right ) const;
        Vec3f  operator-( const float number ) const;
        float  operator*( const Vec3f& right ) const;
        Vec3f  operator*( const float number ) const;
        Vec3f  operator/( const float number ) const;
        Vec3f& operator*=( const float number );
        Vec3f& operator/=( const float number );
	Vec3f& operator+=( const Vec3f& right );
	Vec3f& operator+=( const float number );
	Vec3f& operator-=( const Vec3f& right );
	Vec3f& operator-=( const float number );
	Vec3f  operator-(); // Унарный.
	bool   operator==( const Vec3f& right ) const;
        Vec3f normalize();
	float length();
        ~Vec3f();
};

#endif
