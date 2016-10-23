#include <iostream>
#include "particle_filter.h"

int main()
{
	//Get the configuration parameters
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

	str::particle_filter p_filter(cfg);

}