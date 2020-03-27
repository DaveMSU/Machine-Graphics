#include <iostream>
#include <vector>
#include <cmath>
#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

using namespace std;

int main() {



	vector <Sphere> objects;
	vector <Light> lights;

	float dx;
	float dz;

	for( float phi = 0; phi < 3.14159 * 4; phi += 0.2 ){

		dx = sin(phi);
		dz = cos(phi);
		objects.push_back( Sphere( Vec3f(1.5,-2,7.5), 2, 
				   	Material( Vec3f(0.3,0.6,0.7), 0.15, 0.7, 0.5 ) ) ); // Голубой.
		objects.push_back( Sphere( Vec3f(-1,-1.5,6), 1, 
					Material( Vec3f(0.2,0.7,0.2), 0.15, 0.4, 1.2 ) ) ); // Зелёный.
		objects.push_back( Sphere( Vec3f(-2*dx,1,6.5+2*dz), 1.5, 
					Material( Vec3f(0.7,0.1,0.1), 0.05, 0.5, 0.1 ) ) ); // Красный.
		
		lights.push_back( Light( Vec3f(-10,50,0), 3500 ) );
		lights.push_back( Light( Vec3f(5,5,0), 800 ) );
		lights.push_back( Light( Vec3f(-4,-4,3), 1000 ) );
	
		render( objects, lights );
	
		objects.pop_back();
		objects.pop_back();
		objects.pop_back();

		lights.pop_back();
		lights.pop_back();
		lights.pop_back();
		//break;
	}

	return 0;
}
