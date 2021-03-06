#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "geometry.h" // Кастомная библиотека для работы с геометрическими объектами посредством законов алгебры.
#include "my_lib.h"   // Основная библ-ка, в кот-ой нах-ся описания и справки всех функций.

// Сделать константным!!!!!!!!!!!!!!!!!
std::pair<size_t, float> scene_intersect( const Vec3f& orig,
	           	                  const Vec3f& dir,
                                          const std::vector <Sphere>& spheres ){ 

	float  dist = -1;        // Расстояние до текущего объекта.
	float  min_dist = -1;    // Расстояние до ближайшего объекта.
	size_t min_index  = -1;  // Индекс ближайшего объекта.
	for( size_t i = 0; i < spheres.size(); ++i ){

		dist = spheres[i].ray_intersect( orig, dir );
		if( min_index == -1 && dist != -1 ){		
			min_dist = dist;
			min_index = i;
		}
		else
		if( dist != -1 )
			if( min_dist > dist ){
				min_dist = dist;
				min_index = i;
			}
	}
	return std::pair<size_t, float>(min_index, min_dist);
}

Vec3f cast_ray( const Vec3f& orig, 
		const Vec3f& dir, 
		const std::vector <Sphere>& spheres, 
		const std::vector <Light>& lights,
	     	size_t depth ){

	
	std::pair<size_t, float> index_dist = scene_intersect( orig, dir, spheres );
	size_t min_index = index_dist.first;
	float min_dist   = index_dist.second; 

	if( min_index != -1 ){

		float fin_intensity = 0;//spheres[min_index].material.backg_coef; // Цвет точки на объекте.
		Vec3f point;       // Радиус вектор этой точки.
		Vec3f normal;	   // Нормаль к этой точке.
		Vec3f light_dir;   // Вектор исходящий из источника в эту точку.
		Vec3f spec_dir;	   // Свут источника, отраженный от объекта.
		float diff_coef;   // Коэф. диффузной освещенности.
		float spec_coef;   // Коэф. бликовой (отражающей) освещенности.
		float cos;	   // Переменная в кот-ой хранится cos неко-ых 2ух векторов.
		float distance;    // Расстояние от источника до точки.
		float shadow_coef = 0;      // Степень 'тени', если луч и источника не достигает точки.
		float light_to_indx_point;  // Расстояние от источника света до рассматриваемой точки.

		point = orig + dir*min_dist;
		normal = (point - spheres[min_index].center).normalize();

		Material this_material = spheres[min_index].material;
		for( int i = 0; i < lights.size(); ++i ){

			// Диффузный цвет:
			light_dir = lights[i].position - point;
			cos = (normal * light_dir) / point.length();
			distance = 4*pi * (light_dir * light_dir);
			diff_coef = lights[i].intensity * cos;
			diff_coef /= distance;
   		        diff_coef = std::max(float(0.0), diff_coef);

			// Бликовый цвет:
			spec_dir = normal * 2*(light_dir*normal) - light_dir;
			spec_coef = -point * spec_dir;
			spec_coef /= point.length() * spec_dir.length();
			spec_coef = pow(std::max(float(0.0), spec_coef), 100);

			// Тени:
			Vec3f cur_point;
			for( int indx = 0; indx < spheres.size(); ++indx ){

				if( indx == min_index ) continue;
				light_to_indx_point = spheres[indx]
					.ray_intersect( lights[i].position, -light_dir.normalize() );

				if( (light_to_indx_point > 0) && (light_to_indx_point < light_dir.length()) )
					shadow_coef += 0.08;
			}

			fin_intensity += diff_coef*spheres[min_index].material.diff_coef;
			fin_intensity += spec_coef*spheres[min_index].material.spec_coef;
			fin_intensity *= std::max(float(0.0), 1 - shadow_coef);
		}

		this_material.color *= fin_intensity;

		// Зеркало:
		if( spheres[min_index].material.mirror == true ){// Ошибки с зеркальностью? - см тут.
                                                                      
			if( depth > 0 ){
				Vec3f tmp = cast_ray( point,
					   	      normal + point.normalize()*(normal*point.normalize()), 
						      spheres, 
						      lights,	 
						      depth-1 );	
				if( tmp == background )
					tmp *= 1.05;	
				return tmp * fin_intensity;// Если вдруг буду ошибки с зеркальностью см тут.
			}
		}
		return spheres[min_index].material.color * fin_intensity;
	}
	return background;
}

// Рисуем композицию - если луч пересекает объект, то ставим соотв. цвету объекта пиксель с учетом падения света.
void render( const std::vector <Sphere>& objects, const std::vector <Light>& lights ){

	const int width    = 1024;
	const int height   = 768;
    	std::vector<Vec3f> framebuffer( width * height );

    	for ( size_t j = 0; j < height; j++ )
   	 	for ( size_t i = 0; i < width; i++ ){
			
			float x = (width/(float)height)*(i -  float(width)/2)/float(width);
			float y = -(j - float(height)/2)/float(height);
			Vec3f dir = Vec3f(x,y, 1).normalize(); // +x = направо, +y = вверх, +z = вдаль.
            		framebuffer[i+j*width] = cast_ray( Vec3f(0,0,-5), dir, objects, lights, MAX_DEPTH );
		}
    
    	std::ofstream ofs; // save the framebuffer to file.
    	ofs.open("./out.ppm");
    	ofs << "P6\n" << width << " " << height << "\n255\n";
    	for ( size_t i = 0; i < height*width; ++i ) 
		for( size_t j = 0; j < 3; ++j )
			ofs << (char)( 255 * std::max( 0.f, std::min(1.f, framebuffer[i][j] )) );
    ofs.close();
}



//----------------------------------------Light:-------------------------------------------------------
Light::Light( const Vec3f& p, const float& i ) : position(p), intensity(i) {}



//----------------------------------------Material:-----------------------------------------------------
Material::Material( const Vec3f& color,
	            const float bg, 
		    const float df, 
		    const float sc, 
		    const bool flag ) : 
	color(color), 
	backg_coef(bg), 
	diff_coef(df), 
	spec_coef(sc), 
	mirror(flag) {}// Реализовать зн. по умолчанию!!!!!!!

Material::Material(){}

Material Material::operator*( const float number ) const {   // Умножения материала (пока только цвета) на число.
	Material tmp;
	tmp.color = this->color * number;
	return tmp;
}

float Material::operator*( const Material& right ) const {   // Умножение материала (пока только цвета) на материал (скалярно).
	float scalar;
	scalar = this->color * right.color;
	return scalar;
}

//----------------------------------------Sphere:-------------------------------------------------------
Sphere::Sphere( const Vec3f& c, const float& r, const Material& m ): center(c), radius(r), material(m) {}

// >0 - растояние до пересечения, -1 - не пересекает. (подробней см. my_lib.h)
float Sphere::ray_intersect( const Vec3f& orig, const Vec3f& dir ) const {

        Vec3f L = center - orig;
	
        float cosFI = L*dir / (sqrt(L*L) * sqrt(dir*dir));// cos(dir,радиус-вектор ц/c).
	float r2 = radius*radius;
        float d2 = L*L*(1 - cosFI*cosFI);
        if( d2 > r2 )
                return -1; // Расстояние до ПРЯМОЙ больше радуса.
        if( cosFI < 0 )
                return -1; // 'Смотрят' в разные полупространства (стороны).
        float dist_to_center2 = L*L; // Квадрат расстояния от источника света до центра сферы.
	float dist_d = sqrt(dist_to_center2 + d2); // 'почти' Расстояние до сферы.
	float d = sqrt(r2 - d2); // То самое 'почти'.
	return dist_d - d;
}

