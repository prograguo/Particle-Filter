#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <unistd.h> // usleep

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>
#include <libconfig.h++>

#include "gnuplot-iostream.h"
#include "types.h"
#include "str_io.h"
#include "str_sensor_model.h"
#include "helper_functions.h"

#include "bee-map.h"
#include "bee-map-impl.h"

int main() {
	
	libconfig::Config cfg;

	try{
		cfg.readFile("config/params.cfg");
	}catch(const libconfig::FileIOException &fioex){
		std::cerr << "I/O error while reading file\n";
		return 0;
	}catch(const libconfig::ParseException &pex){
		std::cerr << "Parse error at " << pex.getFile() <<
		 ":" << pex.getLine() << " - " << pex.getError() << "\n";
		return 0;
	}

	double uniformParam, decayScale, decayRate, maxParam, rangeSTD;
	try{
	 uniformParam = cfg.lookup("sensorModel.uniformParam");
	 decayScale = cfg.lookup("sensorModel.decayScale");
	 decayRate = cfg.lookup("sensorModel.decayRate");
	 maxParam = cfg.lookup("sensorModel.maxParam");
	 rangeSTD = cfg.lookup("sensorModel.rangeSTD");
	 std::cout << "successfully read in sensorModel parameters\n";

	}catch(const libconfig::SettingNotFoundException &nfex){
		std::cerr << "No 'name' setting in configuration file." << "\n";
		return 0;
	}


	std::vector<str::laser> laserData;
	std::vector<str::odom> odomData;
	str::readRobotData("data/log/ascii-robotdata2.log", laserData,	odomData);
	
	Gnuplot gp;
	gp << "set term x11 0\n";
	for(int j = 0; j < str::MAX_RANGE; j+= 250){
		std::vector<double> sensorModel;
		sensorModel = str::generateSensorModel( 
			uniformParam, decayScale, decayRate, maxParam, j, rangeSTD);

		std::vector<std::pair<int, double>> sensorModelPlot(str::MAX_RANGE);
		
		for(int i = 0; i < str::MAX_RANGE; i++){

			std::pair<int,double> pt(i, sensorModel[i]);
			sensorModelPlot[i] = pt;
		}
		gp << "plot '-' title 'pts_A'\n";
		gp.send1d(sensorModelPlot);
		
	}


	return 0;
}
