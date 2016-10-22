#ifndef __grapher__
#define __grapher__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "types.h"
#include "particle.h"

namespace str{
class Grapher{

private:
	int m_gridSize;
	int m_cellSize;
	int m_radius;
	int m_windowSize;

	sf::VertexArray sfMapArray;
	std::vector<sf::Vertex> sfParticleArray;
	std::vector<sf::Vertex> sfLaserArray;
	sf::CircleShape sfCentroid;

	sf::RenderWindow sfWindow;
public:
	// constructor
	Grapher(int gridSize, int windowSize);

	// Create a grayscale map vector array
	bool setMap(float** map);

	// Create particle vector array from custom particle structure
	bool setParticlePoints(const std::vector<particle>& particles);

	// Create laser vector array from laser readings
	bool setLaserLines(std::vector<int>& ranges, int xc, int yc);

	// Display the laster vector arrays to 
	void updateGraphics();
};


} // ns str



#endif