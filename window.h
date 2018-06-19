#pragma once
#include "sdl_utility.h"
#include "viewer.h"
#include <iostream>
#include "importsheet.h"

#define WINDOW_WIDTH 660
#define WINDOW_HEIGHT 480

class MainWindow {
public:
  MainWindow();

  void loop();
  void draw(float dt);
  void update(float dt);

  //all animations will loop for now
  AnimationViewer& GetViewer();
  void PlayAnimation(const std::string& animName);
private:
  std::unique_ptr<SDL_Window, SDLUtility::SDL_Deleter> _window;
  std::unique_ptr<SDL_Renderer, SDLUtility::SDL_Deleter> _renderer;
  std::shared_ptr<GUIWindow> _gui;
  std::shared_ptr<ImportSheet> _importSheetWindow;
  AnimationViewer _view;

  SDL_Rect view;

  void InitImage();
};