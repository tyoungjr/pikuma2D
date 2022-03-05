#ifndef GAME_H
#define GAME_H

#include "ECS/ECS.h"
#include "SDL.h"
#include <memory>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
  bool isRunning;
  int millisecsPreviousFrame = 0;
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;

  std::unique_ptr<Registry> registry; 

public:
  Game();
  ~Game();
  void Initialize();
  void Run();
  void Setup();
  void ProcessInput();
  void Update();
  void Render();
  void Destroy();

  int windowWidth;
  int windowHeight;
};

#endif