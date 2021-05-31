#include "Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() {
  isRunning = false;
  std::cout << "Game constructor called" << std::endl;
}

Game::~Game() { std::cout << "Game destructor called" << std::endl; }

void Game::Initialize() {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    std::cerr << "Error initialializing SDL" << std::endl;
    return;
  }
  // get the display mode from the system, setting it to full
  SDL_DisplayMode displayMode;
  SDL_GetCurrentDisplayMode(0, &displayMode);
  // windowWidth = displayMode.w;
  // windowHeight = displayMode.h;
  // give fake fullscreen
  windowWidth = 960;
  windowHeight = 720;
  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);

  if (!window) {
    std::cerr << "Error creating SDL window " << std::endl;
    return;
  }

  renderer = SDL_CreateRenderer(window, -1,
                                0); // window pointer , display index and uflags
  if (!renderer) {
    std::cerr << "Error creating SDL Renderer" << std::endl;
    return;
  }
  SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
  isRunning = true;
}

glm::vec2 playerPosition;
glm::vec2 playerVelocity;
void Game::Setup() {
  playerPosition = glm::vec2(10.0, 20.0);
  playerVelocity = glm::vec2(1.0, 0.0);
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
  playerPosition.x += playerVelocity.x;
  playerPosition.y += playerVelocity.y;
}

void Game::Destroy() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Game::Render() {
  SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
  SDL_RenderClear(renderer);

  // Draw a PNG texture
  SDL_Surface *surface = IMG_Load("./assets/images/tank-tiger-right.png");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);

  // what is the destination rectangle that we want to place our texture
  SDL_Rect dstRect = {static_cast<int>(playerPosition.x),
                      static_cast<int>(playerPosition.y), 32, 32};

  // libsdl.org/sdl_rendercopy
  SDL_RenderCopy(renderer, texture, NULL, &dstRect);
  SDL_DestroyTexture(texture);
  SDL_RenderPresent(renderer);
}