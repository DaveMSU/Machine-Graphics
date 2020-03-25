#include <iostream>
#include <vector>
#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

using namespace std;

int main() {

	vector <Sphere> objects;
	objects.push_back( Sphere( Vec3f(2,0,7), 2, Vec3f(0.3,0.6,0.7) ) );      // Синий.
	objects.push_back( Sphere( Vec3f(-1.5,-1,6), 1, Vec3f(0.2,0.7,0.2) ) );  // Зелёный.
	objects.push_back( Sphere( Vec3f(-2,1,4), 0.5, Vec3f(0.7,0.1,0.4) ) );   // Красный.

	render( objects );
	return 0;
}
