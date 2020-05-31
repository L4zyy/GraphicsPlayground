#include <iostream>
#include "Eigen/Dense"
#include "SDL.h"

int main(int argc, char** argv) {
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Quit();

	Eigen::Matrix3d f;

	f = Eigen::Matrix3d::Random();

	std::cout << f << std::endl;

	return 0;
}