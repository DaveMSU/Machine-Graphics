#include <iostream>
#include <vector>
#include <cmath>
#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

using namespace std;

int main() {

	vector <Object*> objects;
	vector <Light> lights;

	float dx;
	float dz;

	for( float phi = 0; phi < 3.14159 * 4; phi += 0.2 ){

		dx = sin(phi);
		dz = cos(phi);

		objects.push_back( new Chessdesk( Vec3f(-1,0,0), -28, 7, 
					Material( Vec3f(0.3, 0.3, 0.3), 0.2, 0.7, 1, false, -1 ),
				        Material( Vec3f(0.7, 0.7, 0.7), 0.2, 0.7, 1, false, -1 )) ); // Правая стена.


		objects.push_back( new Chessdesk( Vec3f(1,0,0), -28, 7, 
					Material( Vec3f(0.3, 0.3, 0.3), 0.2, 0.7, 1, false, -1 ),
				        Material( Vec3f(0.7, 0.7, 0.7), 0.2, 0.7, 1, false, -1 )) ); // Левая стена.

		objects.push_back( new Chessdesk( Vec3f(0,-1,0), -21, 7, 
					Material( Vec3f(0.1, 0.1, 0.1), 0.2, 0.9, 1, false, -1 ),
				        Material( Vec3f(0.7, 0.7, 0.7), 0.2, 0.9, 1, false, -1 )) ); // Потолок.


		objects.push_back( new Chessdesk( Vec3f(0,0,1), -28, 7, 
					Material( Vec3f(0.3, 0.3, 0.3), 0.2, 0.05, 0.001, false, -1 ),
				        Material( Vec3f(0.7, 0.7, 0.7), 0.2, 0.05, 0.001, false, -1 )) ); // Передняя стена.



		objects.push_back( new Chessdesk( Vec3f(0,0,-1), -77, 7, 
					Material( Vec3f(0.3, 0.3, 0.3), 0.2, 0.05, 0.001, false, -1 ),
				        Material( Vec3f(0.7, 0.7, 0.7), 0.2, 0.05, 0.001, false, -1 )) ); // Задняя стена.

		objects.push_back( new Chessdesk( Vec3f(0,1,0), -14, 7, 
					Material( Vec3f(0.1, 0.1, 0.3), 0.2, 0.5, 0.4, false, -1 ),
				        Material( Vec3f(0.6, 0.8, 0.8), 0.2, 0.7, 2, false, -1 )) ); // Доска.

		objects.push_back( new Sphere( Vec3f(3.5,-1,9), 5.5, 
				   	Material( Vec3f(0.9,0.1,0.1), 0.01, 0.4, 0.5, true, -1 ) ) ); // Стеклянный.
		objects.push_back( new Sphere( Vec3f(-6.5,0,4), 1, 
					Material( Vec3f(0.2,0.7,0.2), 0.15, 0.6, 0.2, false, -1 ) ) ); // Зелёный.
		objects.push_back( new Sphere( Vec3f(-2.5,1.5,6.5), 1.5, 
					Material( Vec3f(0.7,0.1,0.1), 0.05, 0.5, 0.1, false, -1 ) ) ); // Красный.
		objects.push_back( new Sphere( Vec3f(6,-2,3.5), 1.5, 
        			   	Material( Vec3f(0.1,0.5,0.7), 0.2, 0.7, 0.3, false, -1 ) ) ); // Голубой.
		objects.push_back( new Sphere( Vec3f(-3,-2.5,4), 2, 
				   	Material( Vec3f(0.3,0.6,0.7), 0.05, 0.5, 0.4, true, 100.333 ) ) ); // Стеклянный.







		lights.push_back( Light( Vec3f(-10,50,0), 55000 ) );
//		lights.push_back( Light( Vec3f(5,5,-1), 1600 ) );
//		lights.push_back( Light( Vec3f(-40,-40,3), 2000 ) );
		lights.push_back( Light( Vec3f(1,1,-400), 3000000 ) );

		render( objects, lights );
		
		while( objects.size() ){	

			delete objects[objects.size()-1];
			objects.pop_back();
		}
		
		while( lights.size() )
			lights.pop_back();
		break;
	}

	return 0;
}
