#include "window.h"
#include "util.h"
#include <stdexcept>

MainWindow::MainWindow() {
  SDL_Init(SDL_INIT_EVERYTHING);
  _window = SDLUtility::CreateUniqueWindow("Animation Editor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
  _renderer = SDLUtility::CreateUniqueRenderer(_window.get());
  if (_renderer.get() == nullptr) {
    char buf[1024];
    sprintf(buf, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    throw std::runtime_error(buf);
  }
  _view = AnimationViewer(_renderer.get());
  SDL_SetRenderDrawColor(_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
  InitImage();

  _gui = std::make_shared<GUIWindow>(_renderer.get(), _window.get(), this);
  _importSheetWindow = std::make_shared<ImportSheet>(_gui, Vector2(), Vector2(100, 100));

  //LoadImageWindow();
}

AnimationViewer& MainWindow::GetViewer() {
  return _view;
}

void MainWindow::PlayAnimation(const std::string& animName) {
  _view.SetNewAnim(animName);
}

void MainWindow::InitImage() {
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags)) {
    char buf[1024];
    sprintf(buf, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    throw std::runtime_error(buf);
  }
}

void MainWindow::loop() {
  SDL_Event ev;
  Timer fps;
  cpu_clock clock;
  clock.setFPS();

  float frames = 0;
  float startTime = SDL_GetTicks();

  auto previous_time = clock.now();
  bool quit = false;

  while (!quit) {

    if (frames == 0) {
      startTime = SDL_GetTicks();
    }

    frames++;
    fps.start();

    Uint32 dt = clock.now() - previous_time;
    previous_time = clock.now();

    clock.lag += dt;

    //quit_game = handle_events(ev);
    if (SDL_PollEvent(&ev)) {
      if (ev.type == SDL_QUIT)
        quit = true;
      else
        quit = false;
    }

    // try to maintain 60 fps
    while (clock.lag >= clock.timestep) {
      clock.lag -= clock.timestep;
      //update code for animations goes here
      
    }
    this->update(dt);
    this->draw(dt);

    //Cap the frame rate
    if (fps.get_ticks() < clock.timestep)
    {
      SDL_Delay((clock.timestep) - fps.get_ticks());
    }

    if (frames == 1) {
      float f = (1000 * frames) / ((SDL_GetTicks() - startTime));
      std::cout << "FPS:" << f << std::endl;
      frames = 0;
    }
  }
  _gui.reset();
  _renderer.reset();
  _window.reset();
  
  IMG_Quit();
  SDL_Quit();
}

void MainWindow::update(float dt) {
  _view.UpdateView(dt);
}

void MainWindow::draw(float dt) {
  _gui->draw();
  _importSheetWindow->draw();
  _view.DrawView(dt, Vector2(0,0), Vector2(100,100));
  SDL_RenderPresent(_renderer.get());
}