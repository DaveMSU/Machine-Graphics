#include <iostream>
#include <vector>
#include <cmath>

#include <unordered_map>
#include <cstdint>

#include "my_lib.h" // Функции трасировки, сохранения композиции и т.д.

const uint32_t RED   = 0x000000FF;
const uint32_t GREEN = 0x0000FF00;
const uint32_t BLUE  = 0x00FF0000;

using namespace std;

int main( int argc, const char** argv ) {

//---------------------------

  std::unordered_map<std::string, std::string> cmdLineParams;

  for(int i=0; i<argc; i++)
  {
    std::string key(argv[i]);

    if(key.size() > 0 && key[0]=='-')
    {
      if(i != argc-1) // not last argument
      {
        cmdLineParams[key] = argv[i+1];
        i++;
      }
      else
        cmdLineParams[key] = "";
    }
  }

  std::string outFilePath = "zout.bmp";
  if(cmdLineParams.find("-out") != cmdLineParams.end())
    outFilePath = cmdLineParams["-out"];

  int sceneId = 0;
  if(cmdLineParams.find("-scene") != cmdLineParams.end())
    sceneId = atoi(cmdLineParams["-scene"].c_str());

  if( sceneId != 1 )
	  return 0; // Нет других сцен кроме 1-ой.

  uint32_t color = 0;
  if(sceneId == 1)
    color = RED;
  else if(sceneId == 2)
    color = RED | GREEN;
  else if(sceneId == 3)
    color = BLUE;

  std::vector<uint32_t> image(512*512);
  for(auto& pixel : image)
    pixel = color;

//---------------------------	
	
  	vector <Object*> objects;
	vector <Light> lights;

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




	objects.push_back( new Sphere( Vec3f(3.5,-1,9), 4, 
			   	Material( Vec3f(0.9,0.1,0.1), 0.01, 0.4, 0.5, true, -1 ) ) ); // Стеклянный.
	objects.push_back( new Sphere( Vec3f(-6.5,0,4), 1, 
				Material( Vec3f(0.2,0.7,0.2), 0.15, 0.6, 0.2, false, -1 ) ) ); // Зелёный.
	objects.push_back( new Sphere( Vec3f(-2.5,2.5,6.5), 1.5, 
				Material( Vec3f(0.7,0.2,0.1), 0.05, 0.5, 0.1, false, -1 ) ) ); // Красный.
	objects.push_back( new Sphere( Vec3f(6,-2,3.5), 1.5, 
        		   	Material( Vec3f(0.1,0.5,0.7), 0.2, 0.7, 0.3, false, -1 ) ) ); // Голубой.
	objects.push_back( new Sphere( Vec3f(-3,-2.5,4), 2, 
			   	Material( Vec3f(0.3,0.6,0.7), 0.05, 0.5, 0.4, true, 1.333 ) ) ); // Стеклянный.


	lights.push_back( Light( Vec3f(-10,50,0), 55000 ) );
//	lights.push_back( Light( Vec3f(5,5,-1), 1600 ) );
//	lights.push_back( Light( Vec3f(-40,-40,3), 2000 ) );
	lights.push_back( Light( Vec3f(1,1,-400), 3000000 ) );

	render( objects, lights, outFilePath );
		
	while( objects.size() ){	

		delete objects[objects.size()-1];
		objects.pop_back();
	}
		
	while( lights.size() )
		lights.pop_back();

	return 0;
}
