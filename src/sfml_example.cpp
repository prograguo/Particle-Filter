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

	std::vector<str::laser> laserData;
	std::vector<str::odom> odomData;
	str::readRobotData("data/log/ascii-robotdata2.log", laserData,	odomData);
	
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
	int laserCnt = 0;

	sf::CircleShape shape(10);
  shape.setFillColor(sf::Color(100, 250, 250));

  sf::VertexArray lArray(sf::Lines, 180*2);


  sf::Font font;
  if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu-font-family/Ubuntu-M.ttf")){
  	return -1;
  }
  sf::Text FPS("FPS",font, 15);

  sf::Clock clock;
  float lastTime = 0;
	while (window.isOpen()){
		sf::Event event;
		
		// use while (window.pollEvent(event)) to make window responsive (max ~200 FPS)
		// use while (true) to make loop run as fast as possible (400 FPS)
    //while (window.pollEvent(event))
    while (true)
    {

      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed){
        window.close();
      }
     
      
      shape.setPosition(sf::Vector2f(cnt%800, 5*cnt/800));
      
      // FPS
      if(cnt %25 == 0){
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

				if(laserCnt < laserData.size())
				{
						std::vector<std::pair<double,double>> xy;
						xy = str::range2Point(laserData[laserCnt].r); 
						for(int i= 0; i < xy.size(); i++)
						{
							// let's put the laser scan on the first random particle
							int px = 400;//pArray[0].position.x;
							int py = 400;//pArray[0].position.y;
							int rx = xy[i].first/100;
							int ry = xy[i].second/100;
							lArray[2*i].position = sf::Vector2f(px,py);
							lArray[2*i+1].position = sf::Vector2f(px+rx,py+ry); 
							lArray[2*i].color = sf::Color::Blue;
							lArray[2*i+1].color = sf::Color::Blue;
						}
				laserCnt++;
				}


	    }
	    
	    window.clear();
      window.draw(vArray);
      window.draw(pArray);
      window.draw(lArray);
      window.draw(shape);
      window.draw(FPS);
      window.display();
      cnt+=1;
    }
  }


	return 0;
}
