#include "Game.h"
#include <iostream>
#include "../ECS/ECS.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <SDL_image.h>
#include "../Logger/Logger.h"

Game::Game() {
	isRunning = false;
	//std::cout << "Game constructor called!" << std::endl;
	Logger::Log("Game constructor called!");
}

Game::~Game() {
	//std::cout << "Game destructor called!" << std::endl;
	Logger::Log("Game destructor called!");
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		//std::cerr << "Error Initializing SDL." << std::endl;
		Logger::Err("Error Initializing SDL.");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);
	windowWidth = 800;
	windowHeight = 600;

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED,  
		SDL_WINDOWPOS_CENTERED, 
		windowWidth, 
		windowHeight, 
		SDL_WINDOW_BORDERLESS
	);
	
	if (!window) {
		//std::cerr << "Error creating SDL window." << std::endl;
		Logger::Err("Error creating SDL window.");
		return;
	}
	renderer = SDL_CreateRenderer(window, - 1, 0);
	
	if (!renderer) {
		//std::cerr << "Error creating SDL renderer." << std::endl;
		Logger::Err("Error creating SDL renderer.");
		return;
	}
	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

	isRunning = true;
}

void Game::Run() {
	Setup();
	int dividend = 0;
	int divisor = 2;
	while (isRunning)
	{
		int createVariableFlag = dividend % divisor;
		ProcessInput();
		Update(createVariableFlag);
		Render();
		dividend = dividend + 1;
	}
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch(sdlEvent.type){
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYDOWN:
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			break;
		}
	}
	
}



void Game::Setup() {

}

void Game::Update(int createVariableFlag) {
	
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	std::cout << "Ticks: " << SDL_GetTicks() << std::endl;
	std::cout << "Time to wait: " << timeToWait << std::endl;
	std::cout << "Create variable flag: " << createVariableFlag << std::endl;

	if (createVariableFlag > 0) {
		std::string extraVariable = "1000";
		std::cout << "Extra variable: " << extraVariable << std::endl;
		int adress = 100;
		std::cout << "Adress: " << &adress << std::endl;
	}
	else {
		int adress = 100;
		std::cout << "Adress: " << &adress << std::endl;

	}

	
	

	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
	
	millisecsPreviousFrame = SDL_GetTicks();
	
	

}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);



	SDL_RenderPresent(renderer);

}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
