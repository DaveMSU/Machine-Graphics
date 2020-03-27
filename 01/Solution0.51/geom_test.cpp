#include <iostream>
#include <cmath>
#include "geometry.h"

using namespace std;

void print_RGB( const vector <float>& tmp_vec ){

	cout << "RGB:";
	for( int i = 0; i < 3; ++i )
		cout << " " << tmp_vec[i];
	cout << endl;
}

void print_RGB( const Vec3f& tmp_vec ){

	cout << "RGB:";
	for( int i = 0; i < 3; ++i )
		cout << " " << tmp_vec[i];
	cout << endl;
}


int main(){

	Vec3f vec;
	cout << "vec" << endl;
	vector <float> coord;
	coord = vec.get_rgb();
	print_RGB(coord);
	
	Vec3f vec2(1,2,0);
	cout << "vec2(1,2,0)" << endl;
	print_RGB(vec2);
	
	vec = Vec3f(3,8,1);
	cout << "vec = Vec3f(3,8,1)" << endl;
	print_RGB(vec);

	vec = vec + vec2;
	cout << "vec = vec + vec2" << endl;
	print_RGB(vec);	

	float scalar = vec * vec2;
	cout << "vec * vec2 = " << scalar << endl;
	
	vec = vec - 1;
	cout << "vec = vec - 1" << endl;
	print_RGB(vec);

	vec2 = vec2 + 1;
	cout << "vec2 = vec2 + 1" << endl;
	print_RGB(vec2);

	vec2 = vec2 / 2.5;
	cout << "vec2 = vec2 / 2.5" << endl;
	print_RGB(vec2);

	int alpha = 2;
	vec = Vec3f(4,2,4) / alpha;
	cout << "vec = Vec3f(4,2,4) / 2" << endl;
	print_RGB(vec);

	vec2 = Vec3f(0.5, 1, 1.5) * alpha;
	cout << "vec2 = Vec3f(0.5, 1, 1.5) * 2" << endl;
	print_RGB(vec2);

	scalar = (vec * vec) / sqrt(vec2 * vec2);
	cout << "(vec * vec) / sqrt(vec2 * vec2) = " << scalar << endl;

	vec = vec.normalize();
	cout << "vec.normalize()" << endl;
	print_RGB(vec);

	vec *= 3;
	cout << "vec *= 3" << endl;
	print_RGB(vec);

	Vec3f tmp_vec = vec.normalize();
	cout << "tmp = vec.normalize()" << endl;
	print_RGB(tmp_vec);

	float a = 3;
	vec /= 3;
	cout << "vec /= a(=3)" << endl;
	print_RGB(vec);

	cout << "length of vec: " << vec.length() << endl;

	vec += tmp_vec;
	cout << "vec += tmp" << endl;
	print_RGB(vec);

	tmp_vec -= vec;
	cout << "tmp -= vec" << endl;
	print_RGB(tmp_vec);

	cout << "print_RGB(-tmp_vec)" << endl;
	print_RGB(-tmp_vec);

	const Vec3f const_vec(1,2,3);
	cout << "const Vec3f const_vec(1,2,3)" << endl;
	print_RGB(const_vec);
	cout << "const_vec[1] = " << const_vec[1] << endl;

	return 0;
}
