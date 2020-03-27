#include <iostream>
#include <vector>
#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

using namespace std;

int main() {

	vector <Sphere> objects;
	objects.push_back( Sphere( Vec3f(1.5,-0.5,8.5), 2, Vec3f(0.3,0.6,0.7) ) );      // Голубой.
	objects.push_back( Sphere( Vec3f(-0.5,-1.5,7), 1, Vec3f(0.2,0.7,0.2) ) );     // Зелёный.
	objects.push_back( Sphere( Vec3f(-2,1,6.5), 1.5, Vec3f(0.7,0.1,0.1) ) );    // Красный.
	
//	objects.push_back( Sphere( Vec3f(-2,0,8), 2, Vec3f(0.3,0.6,0.7) ) ); 

	vector <Light> lights;
	lights.push_back( Light( Vec3f(-4,50,0), 3500 ) );
	lights.push_back( Light( Vec3f(5,5,0), 800 ) );
	lights.push_back( Light( Vec3f(-4,-4,8), 100 ) );

	render( objects, lights );
	return 0;
}
