#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib> // ex-handling, rand
#include <unistd.h> // usleep

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>

#include <libconfig.h++>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "gnuplot-iostream.h"
#include "types.h"
#include "str_io.h"
#include "str_sensor_model.h"
#include "helper_functions.h"

#include "bee-map.h"
#include "bee-map-impl.h"

int main() {
	
	map_type costMap;
	std::vector<std::pair<int, int>> freeSpace;
	char datLoc[] = "data/map/wean.dat";
	int val = read_beesoft_map(datLoc, &costMap);

	unsigned int width = costMap.size_x;
	unsigned int height = costMap.size_y;

	sf::VertexArray vArray(sf::Points, width*height);

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
	{
	  for (unsigned int j = 0; j < height; ++j)
	  {
	  	vArray[i*height+j].position = sf::Vector2f(i,j);
	  	int p = (costMap.prob[i][j]+ 1.0) *255.0/2.0;
	  	vArray[i*height+j].color = sf::Color(p,p,p);

	  	if(costMap.prob[i][j] == 0){
	  		freeSpace.push_back(std::pair<int, int>(i,j));
	  	}
	  }
	}

	int N_Particles = 1000;
	sf::VertexArray pArray(sf::Points, N_Particles);
	for (unsigned int i = 0; i < N_Particles; i++)
	{
		std::pair<int, int> pt;
		int r_pt = (std::rand() * freeSpace.size()) / RAND_MAX ;
		//std::cout << "r_pt: " << r_pt << "\n";
		pt = freeSpace[r_pt];
		pArray[i].position = sf::Vector2f(pt.first, pt.second);
		pArray[i].color = sf::Color::Red;
	}
	sf::RenderWindow window(sf::VideoMode(800, 800), "sensorModel");
	int cnt = 0;

	sf::CircleShape shape(10);
  shape.setFillColor(sf::Color(100, 250, 250));


  sf::Font font;
  if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/times.ttf")){
  	return -1;
  }
  sf::Text FPS("FPS",font, 10);

  sf::Clock clock;
  float lastTime = 0;
	while (window.isOpen()){
		sf::Event event;
		
		// use while (window.pollEvent(event)) to make window responsive (max ~200 FPS)
		// use while (true) to make loop run as fast as possible (400 FPS)
    while (window.pollEvent(event))
    {

      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed){
        window.close();
      }
     
      
      shape.setPosition(sf::Vector2f(cnt%800, 5*cnt/800));
      
      // FPS
      if(cnt %250 == 0){
	      sf::Time elapsed1 = clock.getElapsedTime();
	      float fps = 25.0 / elapsed1.asSeconds();
	      FPS.setString(std::to_string(fps));
	      clock.restart();

	      for (unsigned int i = 0; i < N_Particles; i++)
				{
					std::pair<int, int> pt;
					int r_pt = (std::rand() * freeSpace.size()) / RAND_MAX ;
					//std::cout << "r_pt: " << r_pt << "\n";
					pt = freeSpace[r_pt];
					pArray[i].position = sf::Vector2f(pt.first, pt.second);
				}
	    }
	    
	    window.clear();
      window.draw(vArray);
      window.draw(pArray);
      window.draw(shape);
      window.draw(FPS);
      window.display();
      cnt+=10;
    }
  }


	return 0;
}
