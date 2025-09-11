#include "Game.h"
#include <iostream>
#include <fstream>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <SDL_image.h>
#include "../Logger/Logger.h"

Game::Game() {
	isRunning = false;
	//std::cout << "Game constructor called!" << std::endl;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
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
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);


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




void Game::LoadLevel(int level) {
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();

	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;
	const int zIndexBackground = 0;

	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, zIndexBackground, srcRectX, srcRectY);
		}
	}
	mapFile.close();




	Entity tank = registry->CreateEntity();

	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", tileSize, tileSize, 2);

	Entity truck = registry->CreateEntity();

	truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(30.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", tileSize, tileSize, 1);
}

void Game::Setup() {
	LoadLevel(1);
}

void Game::Update(int createVariableFlag) {
	
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	//std::cout << "Ticks: " << SDL_GetTicks() << std::endl;
	//std::cout << "Time to wait: " << timeToWait << std::endl;
	//std::cout << "Create variable flag: " << createVariableFlag << std::endl;

	//if (createVariableFlag > 0) {
	//	std::string extraVariable = "1000";
	//	std::cout << "Extra variable: " << extraVariable << std::endl;
	//	int adress = 100;
	//	std::cout << "Adress: " << &adress << std::endl;
	//}
	//else {
	//	int adress = 100;
	//	std::cout << "Adress: " << &adress << std::endl;

	//}

	
	

	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
	
	millisecsPreviousFrame = SDL_GetTicks();
	
	registry->Update();

	registry->GetSystem<MovementSystem>().Update(deltaTime);


}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);


	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);

	SDL_RenderPresent(renderer);

}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
}
