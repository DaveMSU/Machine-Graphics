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
		objects.push_back( Sphere( Vec3f(3.5,-1,9), 4, 
				   	Material( Vec3f(0.9,0.1,0.1), 0.01, 0.7, 4, true ) ) ); // Стеклянный.
		objects.push_back( Sphere( Vec3f(-3*dx,-1.5,6-3*dz), 1, 
					Material( Vec3f(0.2,0.7,0.2), 0.15, 0.4, 1.2, false ) ) ); // Зелёный.
		objects.push_back( Sphere( Vec3f(-2,1,6.5), 1.5, 
					Material( Vec3f(0.7,0.1,0.1), 0.05, 0.5, 0.1, false ) ) ); // Красный.
		objects.push_back( Sphere( Vec3f(5,-2,3.5), 1.5, 
        			   	Material( Vec3f(0.2,0.5,0.8), 0.15, 0.7, 0.5, false ) ) ); // Голубой.
		objects.push_back( Sphere( Vec3f(-4.5,-2,7.5), 1.5, 
				   	Material( Vec3f(0.3,0.6,0.7), 0.15, 0.7, 2, true ) ) ); // Стеклянный.


		lights.push_back( Light( Vec3f(-10,50,0), 3500 ) );
		lights.push_back( Light( Vec3f(5,5,0), 800 ) );
		lights.push_back( Light( Vec3f(-4,-4,3), 1000 ) );
	
		render( objects, lights );
		
		while( objects.size() )	
			objects.pop_back();
		
		while( lights.size() )
			lights.pop_back();
		break;
	}

	return 0;
}
