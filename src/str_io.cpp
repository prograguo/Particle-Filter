//Definiton of str_io

#include "str_io.h"

namespace str
{

	/* 
	Function to read robot data from file
	*/
	void readRobotData(
		std::string data_path, 
		std::vector<laser>& laserData, 
		std::vector<odom>& odomData )
	{
		std::ifstream file;
		std::string line;
		char controlFlag;
		double x, y, theta, ts;

		file.open(data_path);
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			ss >> controlFlag;
			if(controlFlag == 'L'){
				// handle laser data
				laser lData;
				ss >> x >> y >> theta;
				lData.x = x;
				lData.y = y;
				lData.theta = theta;
				ss >> x >> y >> theta;
				lData.xl = x;
				lData.yl = y;
				lData.thetal = theta;
				std::vector<int> r(180);
				for(int i = 0; i < 180; i++){
					ss >> r[i];
				}
				lData.r = r;
				ss >> ts;
				lData.ts = ts;
				laserData.push_back(lData);

			}else{
				//handle odom data
				ss >> x >> y >> theta >> ts;
				odom oData;
				oData.x_cm = x;
				oData.y_cm = y;
				oData.theta_rad = theta;
				oData.ts = ts;
				odomData.push_back(oData);
			}
		}
	}

/* Function to read in map
	*/

	void readMap(
		std::vector<std::vector<float>>& grid, 
		std::string filename)
	{
		std::ifstream file;
		std::string line;
		grid.clear();
		file.open(filename);
		for(int i = 0; i < 7; i++){
			std::getline(file, line);
		}

		while(std::getline(file, line)){
			std::vector<float> rowData;
			std::istringstream ss(line);
			int temp;
			for(int i = 0; i < MAP_SIZE; i++){
				
				ss >> temp;
				rowData.push_back(temp);
			}
			grid.push_back(rowData);
		}
		return;
	}
}