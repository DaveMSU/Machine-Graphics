#include <iostream>
#include <vector>
#include <cmath> // Для математических операций (sqrtf, ...).
#include "geometry.h"

//-----------------------------------Vec3f:-------------------------------------------------

Vec3f::Vec3f(){ // Инициадизируем дефотлный (черный) вектор цветов.

	rgb[0] = 0.0;
	rgb[1] = 0.0;
	rgb[2] = 0.0;
}

Vec3f::Vec3f( float red, float green, float blue ){ // Инициадизируем цвета.

	rgb[0] = red;
	rgb[1] = green;
	rgb[2] = blue;
}

std::vector <float> Vec3f::get_rgb(){
	
	std::vector <float> tmp_rgb = {rgb[0], rgb[1], rgb[2]};
	return tmp_rgb;
}

float& Vec3f::operator[]( int index ){ // Перегрузка оператора разыменования.

	if( index > 2 || index < 0 )
		throw "out of index";
	return rgb[index];
}

Vec3f Vec3f::operator+( const Vec3f& right ) const { // Перегрузка оператора сложения с вектором.(константный)

	Vec3f tmp;
	for( int i = 0; i < 3; ++i )
		tmp.rgb[i] = this->rgb[i] + right.rgb[i];
	return tmp;
}

Vec3f Vec3f::operator+( const float number ) const { // Перегрузка оператора сложения с вектором (убрал &, для obj*3.14).(константный)

	Vec3f tmp;
	for( int i = 0; i < 3; ++i )
		tmp.rgb[i] = this->rgb[i] + number;
	return tmp;
}

Vec3f Vec3f::operator-( const Vec3f& right ) const { // Перегрузка оператора вычитания с вектором.(константный)

	Vec3f tmp;
	for( int i = 0; i < 3; ++i )
		tmp.rgb[i] = this->rgb[i] - right.rgb[i];
	return tmp;
}

Vec3f Vec3f::operator-( const float number ) const { // Перегрузка оператора вычитания с числом.(константный)

	Vec3f tmp;
	for( int i = 0; i < 3; ++i )
		tmp.rgb[i] = this->rgb[i] - number;
	return tmp;
}

float Vec3f::operator*( const Vec3f& right ) const { // Перегрузка оператора умножения на вектор.(константный)

	float tmp = 0;
	for( int i = 0; i < 3; ++i )
		tmp += this->rgb[i] * right.rgb[i];
	return tmp;
}

Vec3f Vec3f::operator*( const float number ) const { // Перегрузка оператора умножения на число.(константный)

	Vec3f tmp;
	for( int i = 0; i < 3; ++i )
		tmp.rgb[i] = this->rgb[i] * number;
	return tmp;
}

Vec3f Vec3f::operator/( const float number ) const { // Перегрузка оператора деления на число.(константный)

	Vec3f tmp;
	for( int i = 0; i < 3; ++i )
		tmp.rgb[i] = this->rgb[i] / number;
	return tmp;
}

void Vec3f::normalize(){ // Приведение к нормированному виду с нормой = 1.

	float norma = 0;
	for( int i = 0; i < 3; ++i )
		norma += this->rgb[i] * this->rgb[i];
	norma = sqrt(norma);

	for( int i = 0; i < 3; ++i )
		this->rgb[i] /= norma;
}
	
Vec3f::~Vec3f(){}; // Деструктор.
