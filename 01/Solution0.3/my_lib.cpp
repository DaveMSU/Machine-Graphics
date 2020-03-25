#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "geometry.h" // Кастомная библиотека для работы с геом-ми объектами законами алгеборы.
#include "my_lib.h" // Основная библ-ка, в кот-ой нах-ся описания и справки всех функций.

bool scene_intersect( const Vec3f& orig, const Vec3f& dir, std::vector <Sphere> spheres, Material& this_material ){ // Вернуть const!

	bool flag = false;
	for( int i = 0; i < spheres.size(); ++i ){

		if( spheres[i].ray_intersect( orig, dir ) )
			this_material = spheres[i].material;
		flag = true;
	}
	return flag;
}

Vec3f cast_ray( const Vec3f& orig, const Vec3f& dir, std::vector <Sphere> spheres ){ // Поставить const!!!!! и &!!!!!

	Material this_material;
	if( scene_intersect( orig, dir, spheres, this_material ) )
		return this_material.diffuse_color; // Фон.
	return Vec3f( 0.2, 0.7, 0.8 );
}

void render( const std::vector <Sphere>& objects ) { // Для отрисовки самой простой композиции - пересек. или нет.

	const int width    = 1024;
	const int height   = 768;
    	std::vector<Vec3f> framebuffer( width * height );

    	for ( size_t j = 0; j < height; j++ )
   	 	for ( size_t i = 0; i < width; i++ ){
		
            		
			//float x =  (2*(i + 0.5)/(float)width  - 1)*tan(fov/2.)*width/(float)height;
		        //float y = -(2*(j + 0.5)/(float)height - 1)*tan(fov/2.);
			float x = (width/(float)height)*(i -  float(width)/2)/float(width);
			float y = (j - float(height)/2)/float(height);
			Vec3f dir = Vec3f(x, y, -1);
			dir.normalize(); // Поменять!!!!!!
            		framebuffer[i+j*width] = cast_ray( Vec3f(0,0,0), dir, objects );
		}
    
    	std::ofstream ofs; // save the framebuffer to file.
    	ofs.open("./out.ppm");
    	ofs << "P6\n" << width << " " << height << "\n255\n";
    	for ( size_t i = 0; i < height*width; ++i ) 
		for( size_t j = 0; j < 3; ++j )
			ofs << (char)( 255 * std::max( 0.f, std::min(1.f, framebuffer[i][j] )) );
    ofs.close();
}





//----------------------------------------Material:-----------------------------------------------------
Material::Material( const Vec3f& color ) : diffuse_color(color) {}

Material::Material() : diffuse_color() {}

//Перегрузка оператора присваивания.
//Material& Material::operator=( const Material& right ){
//
//	this->rgb[0] = right.rgb[0];
//        this->rgb[1] = right.rgb[1];
//	this->rgb[2] = right.rgb[2];
//
//	return *this;
//}


//----------------------------------------Sphere:-------------------------------------------------------
Sphere::Sphere( const Vec3f& c, const float& r, const Material& m ): center(c), radius(r), material(m) {}

bool Sphere::ray_intersect( const Vec3f& orig, const Vec3f& dir ){

        Vec3f L = center - orig;
	
        float cosFI = L*dir / (sqrt(L*L) * sqrt(dir*dir));// cos(dir,радиус-вектор ц/c).
        float d2 = L*L*(1 - cosFI*cosFI);
        if( d2 > radius*radius )
                return false; // Расстояние до ПРЯМОЙ больше радуса.
        if( cosFI < 0 )
                return false; // 'Смотрят' в разные полупространства (стороны).
        return true;
}

