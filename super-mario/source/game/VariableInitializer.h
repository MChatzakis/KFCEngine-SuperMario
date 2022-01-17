#ifndef _VARIABLEINITIALIZER_H_
#define _VARIABLEINITIALIZER_H_

#include "./GameVars.h"

void initializeVariables() {
	nlohmann::json config = readJSON("resources/config/config.json");
	if (config == NULL) {
		std::cout << "Cannot open configuration file!" << std::endl;
		exit(-1);
	}

	SCREEN_WIDTH = config["screen"]["width"];
	SCREEN_HEIGHT = config["screen"]["height"];


	viewWin = new Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	gridWin = new Rect(0, 0, 20, 20);
	game = new Game();
}

#endif _VARIABLEINITIALIZER_H_