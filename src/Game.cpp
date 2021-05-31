#include "Game.h"
#include <SDL2/SDL.h>
#include <iostream>

Game::Game() {
  // TODO...
  std::cout << "Game constructor called" << std::endl;
}

Game::~Game() { std::cout << "Game destructor called" << std::endl; }

void Game::Initialize() {
  if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
    std::cerr << "Error initialializing SDL" << std::endl;
    return;
  }

  SDL_Window* window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED, 
    SDL_WINDOWPOS_CENTERED,
    800,
    600,
    SDL_WINDOW_BORDERLESS); 

  if (!window) {
    std::cerr << "Error creating SDL window " << std::endl;
    return;
  }

  SDL_Renderer* renderer = SDL_CreateRenderer(window,-1,0 );// window pointer , display index and uflags
  if (!renderer) {
    std:cerr << "Error creating SDL Renderer" << std::endl; 
    return;
  } 
}

void Game::Run() {
  // while (true) {
  //   ProcessInput();
  //   Update();
  //   Render();
  // }
}

void Game::ProcessInput() {}

void Game::Update() {}

void Game::Destroy() {}

void Game::Render() {}