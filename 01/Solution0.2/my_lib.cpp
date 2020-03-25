#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "geometry.h" // Кастомная библиотека для работы с геом-ми объектами законами алгеборы.
#include "my_lib.h" // Основная библ-ка, в кот-ой нах-ся описания и справки всех функций.

void render() { // Для отрисовки картинки-примера.
    	const int width    = 1024;
	const int height   = 768;
    	std::vector<Vec3f> framebuffer(width*height);

    	for (size_t j = 0; j<height; j++)
   	 	for (size_t i = 0; i<width; i++)
		    	framebuffer[i+j*width] = Vec3f(j/float(height), i/float(width), 0);
    
    	std::ofstream ofs; // save the framebuffer to file.
    	ofs.open("./out.ppm");
    	ofs << "P6\n" << width << " " << height << "\n255\n";
    	for (size_t i = 0; i < height*width; ++i) 
		for(size_t j = 0; j < 3; ++j)
			ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
    ofs.close();
}

Vec3f cast_ray( const Vec3f& orig, const Vec3f& dir, Sphere sphere ){ // Поставить const!!!!!
	
	if( !sphere.ray_intersect( orig, dir ) )
		return Vec3f(0.2, 0.7, 0.8); // Фон.
	return Vec3f( 0.4, 0.4, 0.3 );
}

void render( const Sphere& sphere ) { // Для отрисовки самой простой композиции - пересек. или нет.

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
            		framebuffer[i+j*width] = cast_ray( Vec3f(0,0,0), dir, sphere );
		}
    
    	std::ofstream ofs; // save the framebuffer to file.
    	ofs.open("./out.ppm");
    	ofs << "P6\n" << width << " " << height << "\n255\n";
    	for ( size_t i = 0; i < height*width; ++i ) 
		for( size_t j = 0; j < 3; ++j )
			ofs << (char)( 255 * std::max( 0.f, std::min(1.f, framebuffer[i][j] )) );
    ofs.close();
}

Sphere::Sphere( const Vec3f &c, const float &r ): center(c), radius(r) {}

bool Sphere::ray_intersect( const Vec3f& orig, const Vec3f& dir ){

        Vec3f L = center - orig;
	
        float cosFI = L*dir / (sqrt(L*L) * sqrt(dir*dir));// cos(dir,радиус-вектор ц/c).
        float d2 = L*L*(1 - cosFI*cosFI);
        if( d2 > radius*radius )
                return false; // Расстояние до ПРЯМОЙ больше радуса.
//        if( cosFI < 0 )
//                return false; // 'Смотрят' в разные полупространства (стороны).
        return true;
}

