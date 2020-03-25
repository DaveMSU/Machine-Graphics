#include <iostream>
#include <vector>
#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

using namespace std;

int main() {


	vector <Sphere> objects;
	objects.push_back( Sphere( Vec3f(0,1,-9), 2.5, Vec3f(0.3,0.6,0.7) ) );
	objects.push_back( Sphere( Vec3f(-1,0,-9), 1.5, Vec3f(0.2,0.7,0.2) ) );
	objects.push_back( Sphere( Vec3f(-3,1,-4), 1, Vec3f(0.7,0.1,0.4) ) );

	render( objects );
	return 0;
}
