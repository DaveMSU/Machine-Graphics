#include <iostream>
#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

int main() {

	Sphere sph1( Vec3f(0,1,-4), 1 );
	render(sph1);
	return 0;
}
