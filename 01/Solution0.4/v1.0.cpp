#include <iostream>
#include <vector>
#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

using namespace std;

int main() {

	vector <Sphere> objects;
	objects.push_back( Sphere( Vec3f(2,-0.5,8), 2, Vec3f(0.3,0.6,0.7) ) );      // Голубой.
	objects.push_back( Sphere( Vec3f(-1,-1.5,7), 1, Vec3f(0.2,0.7,0.2) ) );     // Зелёный.
	objects.push_back( Sphere( Vec3f(-1,1.5,8), 2.5, Vec3f(0.3,0.1,0.7) ) );    // Фиолетовый.

	vector <Light> lights;
	lights.push_back( Light( Vec3f(-4,3,-123456), 123456 ) );

	render( objects, lights );
	return 0;
}
