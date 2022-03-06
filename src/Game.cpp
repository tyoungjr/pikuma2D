#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>

#include "ECS/ECS.h"
#include "Game/Game.h"
#include "Logger/Logger.h"
#include "Systems/MovementSystem.h"
#include "Components/TransformComponent.h"
#include "Components/RigidBodyComponent.h"
#include "Components/SpriteComponent.h"
#include "Systems/MovementSystem.h"
#include "Systems/RenderSystem.h"

Game::Game() {
	isRunning = false;
	registry = std::make_unique<Registry>(); 
	Logger::Log("Game constructor called!");
}

Game::~Game() { Logger::Log("Game destructor called"); }

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL");
		return;
	}
	// get the display mode from the system, setting it to full
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	// TODO Dynamic resolution based on display mode 
	windowWidth = 960;
	windowHeight = 720;
	window = SDL_CreateWindow("Pikuma 2D Engine Test",
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight, 0);

	if (!window) {
		Logger::Err("Error creating SDL window.");
		return;
	}

	
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		Logger::Err("Error creating SDL Renderer");
		return;
	}
	// If you want fake full screen
	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN); 
	isRunning = true;
}

void Game::Setup() {

	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();

	Entity tank = registry->CreateEntity();

	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0); 
	tank.AddComponent<RigidBodyComponent>(glm::vec2(40.0, 0.0)); 
	tank.AddComponent<SpriteComponent>(10, 10);

	Entity truck = registry->CreateEntity();

	truck.AddComponent<TransformComponent>(glm::vec2(50.0, 100.0), glm::vec2(1.0, 1.0), 0.0); 
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 50.0)); 
	truck.AddComponent<SpriteComponent>(10, 50);


}

void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput() {

	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
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

void Game::Update() {
	// if we are too fast , release execution until we reach the
	// MILLISECS_PER_FRAME
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);

	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}

	// the difference in ticks since the last frame, converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	// store the "previous" frame time
	millisecsPreviousFrame = SDL_GetTicks();

	registry->Update();

	registry->GetSystem<MovementSystem>().Update(deltaTime);
	// registry->GetSystem<MovementSystem>().Update();
	// CollisionSystem.Update();
	// DamageSystem.Update();


}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO: Render game object
	registry->GetSystem<RenderSystem>().Update(renderer); 

	SDL_RenderPresent(renderer);
}
