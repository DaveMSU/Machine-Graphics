#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "geometry.h" // Кастомная библиотека для работы с геометрическими объектами посредством законов алгебры.
#include "my_lib.h"   // Основная библ-ка, в кот-ой нах-ся описания и справки всех функций.


// Отражаем вектор vec относительно нормали.
//
Vec3f reflect( const Vec3f& vec, const Vec3f& normal ){ 
	
	return normal * 2 * std::max(0.f, ((-vec)*normal)) + vec;
}


// Для нахождения OY, по OX и normal в Chessdesk.get_material().
//
Vec3f vector_product( const Vec3f& v1, const Vec3f& v2 ){ 

    float x = v1[1]*v2[2] - v1[2]*v2[1];
    float y = v1[2]*v2[0] - v1[0]*v2[2];
    float z = v1[0]*v2[1] - v1[1]*v2[0];

    return Vec3f(x,y,z);
}


// Сделать константным!!!!!!!!!!!!!!!!!
//
std::pair<size_t, float> scene_intersect( const Vec3f& orig,
	           	                  const Vec3f& dir,
                                          const std::vector <Object*>& objects ){ 

	static float  dist;        // Расстояние до текущего объекта.
	static float  min_dist;    // Расстояние до ближайшего объекта.
	static size_t min_index;   // Индекс ближайшего объекта.
	
	dist = -1;
	min_dist = -1;
	min_index = -1;

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
		const Vec3f& dir, 		      // Соглашение: вектор должен приходить нормализованным.
		const std::vector <Object*>& objects, // spheres/chessdesk 
		const std::vector <Light>& lights,
	     	size_t depth ){

	if( fabs(dir.length() - 1.f) > 1E-5 ){ // Соглашение: dir пришел не нормированным - бросаем исключение.

		std::cout << dir.length() << std::endl;
		std::string exc = "\n*****\nFunction: cast_ray()\n";
		exc += "dir пришел не нормализованным.\ncurrent depth: ";
	        exc += std::to_string(depth) + "\n*****";
		throw std::logic_error(exc);
	}

	static std::pair<size_t, float> index_dist;
	static size_t min_index;
	static float min_dist;
	
	index_dist = scene_intersect( orig, dir, objects );
	min_index = index_dist.first;
	min_dist  = index_dist.second; 

	if( min_index != -1 ){

		static Vec3f point;         // Радиус вектор этой точки.
		static Vec3f normal;	    // Нормаль к этой точке.
		static Vec3f light_dir;     // Вектор исходящий из источника в эту точку.
		static Vec3f spec_dir;	    // Свет источника, отраженный от объекта.
		static float diff_coef;     // Коэф. диффузной освещенности.
		static float spec_coef;     // Коэф. бликовой (отражающей) освещенности.
		static float cos;	    // Переменная в кот-ой хранится cos неко-ых 2ух векторов.
		static float distance;      // Расстояние от источника света до точки.
		static float shadow_coef;   // Степень 'тени', если луч источника не достигает точки.
		static float light_to_indx_point;  // Расстояние от источника света до рассм-емой точки.
		static float fin_intensity; // Цвет точки на объекте.

		shadow_coef = 0;	       
	        fin_intensity = objects[min_index]->get_material(point).backg_coef;

		point = orig + dir*min_dist;
		Material this_material = objects[min_index]->get_material(point);
		for( size_t i = 0; i < lights.size(); ++i ){

			// Диффузный цвет:
			//
			light_dir = lights[i].position - point;
			cos = (normal * light_dir) / light_dir.length();
			distance = 4*pi * (light_dir * light_dir);
			diff_coef = lights[i].intensity * cos / distance;
   		        diff_coef = std::max(0.f, diff_coef);

			// Бликовый цвет:
			//
			spec_dir = reflect(-light_dir, normal);
			spec_coef = (-point).normalize() * spec_dir.normalize();
			spec_coef = pow(std::max(0.f, spec_coef), 100);

			// Тени:
			//
			Vec3f cur_point;
			for( size_t indx = 0; indx < objects.size(); ++indx ){

				if( indx == min_index ) continue;
				light_to_indx_point = objects[indx]
					->ray_intersect( lights[i].position, -light_dir.normalize() );

				if( (light_to_indx_point > 0) && (light_to_indx_point < light_dir.length()) )
					shadow_coef += 0.08;
			}

			fin_intensity += diff_coef*objects[min_index]->get_material(point).diff_coef;
			fin_intensity += spec_coef*objects[min_index]->get_material(point).spec_coef;
			fin_intensity *= std::max(0.f, 1 - shadow_coef);
		}

		this_material.color *= fin_intensity;

		// Зеркало:
		//
		static float n2;
	        n2 = objects[min_index]->get_material(point).refr_coef;
		if( objects[min_index]->get_material(point).mirror == true ){
			
			if( n2 > 0 ){

				float R    = ((Sphere*)objects[min_index])->get_radius();
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
				
				Vec3f tmp = cast_ray( point,
						      reflect(dir, normal).normalize(),
						      objects, 
						      lights,	 
						      depth-1 );
				if( tmp == background )
					tmp *= 1.05;
				return tmp * fin_intensity;
			}
	//		return background;
		}
		return objects[min_index]->get_material(point).color * fin_intensity;
	}
	return background;
}


// Рисуем композицию - если луч пересекает объект, то ставим соотв. цвету объекта пиксель с учетом падения света.
//
void render( const std::vector <Object*>& objects, const std::vector <Light>& lights ){

	const int width    = 1024*1;
	const int height   = 768*1;
    	std::vector<Vec3f> framebuffer( width * height );

    	for ( size_t j = 0; j < height; j++ )
   	 	for ( size_t i = 0; i < width; i++ ){
			
			float x = (width/(float)height)*(i -  float(width)/2)/float(width);
			float y = -(j - float(height)/2)/float(height);
			Vec3f dir = Vec3f(x,y, 1).normalize(); // +x = направо, +y = вверх, +z = вдаль.
            		framebuffer[i+j*width] = cast_ray( Vec3f(0,0,-15), dir, objects, lights, MAX_DEPTH );
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

//----------------------------------------Sphere:-------------------------------------------------------
Sphere::Sphere( const Vec3f& c, const float& r, const Material& m ): center(c), radius(r), material(m) {}

// >0 - растояние до пересечения, -1 - не пересекает. (подробней см. my_lib.h)
//
float Sphere::ray_intersect( const Vec3f& orig, const Vec3f& dir ) const {

	if( fabs(dir.length() - 1.f) > 1E-5 ){ // Соглашение: dir пришел не нормированным - бросаем исключение.

		std::string exc = "\n*****\nFunction: Sphere::ray_intersect()\n";
		exc += "dir пришел не нормализованным.\n*****";
		throw std::logic_error(exc);
	}

        static Vec3f L;
	static float cosFI;
	static float r2;
	static float d2;
	static float dist_to_center2;
	static float dist_d;
	static float d;
	
	L = center - orig;
        cosFI = dir*L.normalize(); // cos(dir,радиус-вектор ц/c).
	r2 = radius*radius;
        d2 = L*L*(1 - cosFI*cosFI);
        
	if( d2 > r2 )
                return -1;         // Расстояние до ПРЯМОЙ больше радуса.
        if( cosFI < 0 )
                return -1;         // Р/в центра сферы и вектор направления луча направлены в разные стороны.
        
	dist_to_center2 = L*L;     // Квадрат расстояния от камеры до центра сферы.
	dist_d = sqrt(dist_to_center2 + d2); // 'почти' Расстояние до сферы.
	d = sqrt(r2 - d2);         // То самое 'почти'.

	return dist_d - d;
}


Material Sphere::get_material( const Vec3f& point ) const {

	return material;
}


Vec3f Sphere::get_normal( const Vec3f& point ) const {

	return (point - center).normalize();	
}


float Sphere::get_radius() const {

	return radius;
}

//--------------------------------------Chessdesk:-----------------------------------------------------
Chessdesk::Chessdesk( const Vec3f& n, 
		      const float& b, 
		      const float& size,
		      const Material& m1,
		      const Material& m2 ): 

	normal(n.normalize()), 
	bias(b / n.length()), 
	call_size(size),
	first_material(m1),
	second_material(m2) {
	
		/* Вычисляем некоторую условную точку O - начало координат. */
		O = Vec3f(0,0,0);
		size_t i;
		for( i = 0; !normal[i]; ++i );
        	O[i] = -bias / normal[i];

		// По 3dtmp вычисляем 2dtmp - вектор, но в координатах плоскости.
		//
		c1 = Vec3f(0,0,0);
		c2 = Vec3f(0,0,0);

		Vec3f tmp_vec(0,0,0);
		tmp_vec[(i+2)%3] = normal[i] / fabs(normal[i]);                                      	
	                                                                                     	
		c1 = normal*(bias+normal*tmp_vec)/(normal*normal) + tmp_vec;                         	
		c1 = c1 - O;                                                                         	
		c2 = vector_product(c1,normal); // Векторное произведение.	
}


float Chessdesk::ray_intersect( const Vec3f& orig, const Vec3f& dir ) const {

	if( fabs(dir.length() - 1.f) > 1E-5 ){ // Соглашение: dir пришел не нормированным - бросаем исключение.

		std::string exc = "\n*****\nFunction: Chessdesk::ray_intersect()\n";
		exc += "dir пришел не нормализованным.\n*****";
		throw std::logic_error(exc);
	}

	if( (orig*normal-bias) * (dir*normal) < 0 )
			return (normal*orig+bias)/(dir*normal);
	return -1;		
}


Material Chessdesk::get_material( const Vec3f& point ) const {

	Vec3f tmp3d = point - O; // Вектор точки point расматриваемый относительно плоскости.
	Vec3f tmp2d(tmp3d*c1, tmp3d*c2, 0);
	
	static bool xd;
	static bool yd;

	xd = (tmp2d[0] - floor(tmp2d[0]/(call_size*2))*2*call_size) < call_size;
	yd = (tmp2d[1] - floor(tmp2d[1]/(call_size*2))*2*call_size) < call_size;

	if( xd == yd )
		return first_material;
	else
		return second_material;
}


Vec3f Chessdesk::get_normal( const Vec3f& point ) const {

	return normal;	
}
