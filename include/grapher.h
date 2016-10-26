#ifndef __grapher__
#define __grapher__

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "types.h"
#include "particle.h"
#include <memory>

namespace str{

class Grapher{

private:
	int m_gridSize;
	int m_cellSize;
	int m_radius;
	int m_windowSize;
	sf::VertexArray sfMapArray;
	std::vector<sf::CircleShape> sfParticleShapeArray;
	std::vector<sf::Vertex> sfParticleDirArray;
	std::vector<sf::Vertex> sfLaserArray;
	sf::CircleShape sfCentroid;
	sf::RenderWindow sfWindow;

	// secondary sensor window params
	int m_sensorWindowSize;
	sf::RenderWindow sfSensorWindow;
	std::vector<sf::Vertex> sfMeasRanges;
	std::vector<sf::Vertex> sfPredRanges;

public:
	// constructor
	Grapher(int gridSize, int windowSize);

	Grapher(int gridSize, int windowSize, int sensorWindowSize);

	// Grapher(Grapher &obj);

	// Create a grayscale map vector array
	bool setMap(float** map);

	// Create particle vector array from custom particle structure
	bool setParticlePoints(const str::particles& particles);
	bool setParticlePoints(const str::particles& particles, 
		double m_len, double m_size);

	// Create laser vector array from laser readings
	bool setLaserLines(std::vector<int>& ranges, str::particle& p);

	void setCentroid(int xc, int yc);

	// Display the latest vector arrays on MAP graphics
	void updateGraphics();

	// set laser Reading for Sensor Window
	bool setMeasuredRanges(const std::vector<int>& ranges);

	bool setPredictedRanges(const std::vector<int>& ranges);

	// Display the latest sensor reading on SENSOR graphics
	void updateSensorGraphics();
};


} // ns str



#endif