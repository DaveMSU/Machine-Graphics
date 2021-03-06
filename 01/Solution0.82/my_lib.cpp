#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "geometry.h" // Кастомная библиотека для работы с геометрическими объектами посредством законов алгебры.
#include "my_lib.h"   // Основная библ-ка, в кот-ой нах-ся описания и справки всех функций.

Vec3f reflect( const Vec3f& vec, const Vec3f& normal ){ // Отражаем вектор vec относительно нормали normal.
	
	return normal * 2 * (std::max(float(0.0),(-vec)*normal)) + vec;
}

// Сделать константным!!!!!!!!!!!!!!!!!
std::pair<size_t, float> scene_intersect( const Vec3f& orig,
	           	                  const Vec3f& dir,
                                          const std::vector <Object*>& objects ){ 

	float  dist = -1;        // Расстояние до текущего объекта.
	float  min_dist = -1;    // Расстояние до ближайшего объекта.
	size_t min_index  = -1;  // Индекс ближайшего объекта.
	for( size_t i = 0; i < objects.size(); ++i ){

		dist = objects[i]->ray_intersect( orig, dir );
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
		const std::vector <Object*>& objects, // spheres/chessdesk 
		const std::vector <Light>& lights,
	     	size_t depth ){

	std::pair<size_t, float> index_dist = scene_intersect( orig, dir, objects );
	size_t min_index = index_dist.first;
	float min_dist   = index_dist.second; 

	if( min_index != -1 ){

		// Сделать все переменные static.
		float fin_intensity = objects[min_index]->material.backg_coef; // Цвет точки на объекте.
		Vec3f point;       // Радиус вектор этой точки.
		Vec3f normal;	   // Нормаль к этой точке.
		Vec3f light_dir;   // Вектор исходящий из источника в эту точку.
		Vec3f spec_dir;	   // Свет источника, отраженный от объекта.
		float diff_coef;   // Коэф. диффузной освещенности.
		float spec_coef;   // Коэф. бликовой (отражающей) освещенности.
		float cos;	   // Переменная в кот-ой хранится cos неко-ых 2ух векторов.
		float distance;    // Расстояние от источника света до точки.
		float shadow_coef = 0;      // Степень 'тени', если луч источника не достигает точки.
		float light_to_indx_point;  // Расстояние от источника света до рассматриваемой точки.

		// Добавить проверку на сферу.!!!!!
		point = orig + dir.normalize()*min_dist; // Нормализуем, т.к. по дефолту не норм-но в рекурсии.
		normal = (point - ((Sphere*)objects[min_index])->center).normalize();

		Material this_material = objects[min_index]->material;
		for( int i = 0; i < lights.size(); ++i ){

			// Диффузный цвет:
			light_dir = lights[i].position - point;
			cos = (normal * light_dir) / light_dir.length();// / point.length(); !!!!!!!!!
			distance = 4*pi * (light_dir * light_dir);
			diff_coef = lights[i].intensity * cos / distance;
   		        diff_coef = std::max(float(0.0), diff_coef);

			// Бликовый цвет:
			spec_dir = reflect(-light_dir, normal);
			spec_coef = (-point).normalize() * spec_dir.normalize();
			spec_coef = pow(std::max(float(0.0), spec_coef), 100);

			// Тени:
			Vec3f cur_point;
			for( int indx = 0; indx < objects.size(); ++indx ){

				if( indx == min_index ) continue;
				light_to_indx_point = objects[indx]
					->ray_intersect( lights[i].position, -light_dir.normalize() );

				if( (light_to_indx_point > 0) && (light_to_indx_point < light_dir.length()) )
					shadow_coef += 0.08;
			}

			fin_intensity += diff_coef*objects[min_index]->material.diff_coef;
			fin_intensity += spec_coef*objects[min_index]->material.spec_coef;
			fin_intensity *= std::max(float(0.0), 1 - shadow_coef);
		}

		this_material.color *= fin_intensity;

		// Зеркало:
		float n2 = objects[min_index]->material.refr_coef;
		if( objects[min_index]->material.mirror == true ){ // Добавить проверку на сферу!!!!!!
                    
			if( n2 > 0 ){

                                /*
				float R = spheres[min_index].radius;
                                float cosA = ((-point)*normal)/point.length();
                                float cosB = sqrt( 1 - (1 - cosA*cosA)/(n2*n2) );
                                float point_coef = 1/(n2*point.length());
                                float normal_coef = cosA/n2 - cosB;
                                float main_coef = 2*cosB*R;
                                Vec3f tmp_vec = (point*point_coef + normal*normal_coef)*main_coef;
                                Vec3f tmp_normal = ((-normal)*R - tmp_vec/2).normalize();
				*/

				// Добавить проверку на сферу.!!!!!
				float R    = ((Sphere*)objects[min_index])->radius;
				float cosA = (-point).normalize()*normal;
				float cosB = sqrt( 1 - (1/(n2*n2)) * (1-cosA*cosA) );
				float sinA = sqrt(1 - cosA*cosA);
				float sinB = sqrt(1 - cosB*cosB);
				
				float ABs  = 2*R*cosB*cosB;
				Vec3f ABv  = -normal;
				float BCs  = sinB*2*R*cosB;
				Vec3f BCv  = (point.normalize() + normal*cosA)*(1/sinA); //!!!

				Vec3f in_vec  = ABv*ABs + BCv*BCs;
				Vec3f tmp_vec = (-normal)*R - in_vec*0.5;
				//Vec3f out_vec = reflect(point, BCv).normalize(); 
				Vec3f out_vec = reflect(in_vec, BCv).normalize(); 

				if( depth > 0 ){
					Vec3f tmp = cast_ray( point+in_vec,
						   	      out_vec, 
							      objects, 
							      lights,	 
							      depth-1 );	
					if( tmp == background )
						tmp *= 1.05;	
					return tmp * fin_intensity;
				}
			}
			else
			if( depth > 0 ){
				normal = normal.normalize();
				Vec3f tmp = cast_ray( point,
						      reflect(point, normal),
						      objects, 
						      lights,	 
						      depth-1 );	
				if( tmp == background )
					tmp *= 1.05;	
				return tmp * fin_intensity;
			}
		}
		return objects[min_index]->material.color * fin_intensity;
	}
	return background;
}

// Рисуем композицию - если луч пересекает объект, то ставим соотв. цвету объекта пиксель с учетом падения света.
void render( const std::vector <Object*>& objects, const std::vector <Light>& lights ){

	const int width    = 1024*1;
	const int height   = 768*1;
    	std::vector<Vec3f> framebuffer( width * height );

    	for ( size_t j = 0; j < height; j++ )
   	 	for ( size_t i = 0; i < width; i++ ){
			
			float x = (width/(float)height)*(i -  float(width)/2)/float(width);
			float y = -(j - float(height)/2)/float(height);
			Vec3f dir = Vec3f(x,y, 1).normalize(); // +x = направо, +y = вверх, +z = вдаль.
            		framebuffer[i+j*width] = cast_ray( Vec3f(0,0,-9), dir, objects, lights, MAX_DEPTH );
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
		    const bool flag,
		    const float rf ) : 
	color(color), 
	backg_coef(bg), 
	diff_coef(df), 
	spec_coef(sc), 
	mirror(flag),
        refr_coef(rf){}// Реализовать зн. по умолчанию!!!!!!!

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

        float cosFI = L.normalize()*dir.normalize();// cos(dir,радиус-вектор ц/c).
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

//--------------------------------------Chessdesk:-----------------------------------------------------
Chessdesk::Chessdesk( const Vec3f& n, 
		      const float& b, 
		      const float& size,
		      const Material& m): 
	normal(n), 
	bias(b), 
	call_size(size),
	material(m) {}

float Chessdesk::ray_intersect( const Vec3f& orig, const Vec3f& dir ) const {

	Vec3f tmp = normal.normalize();
	if( orig.normalize()*tmp > 0 )
		return -(tmp*orig+bias)/(dir.normalize()*tmp); // Убрать normalize()'ы.!!!!!

	return -1;		
}
