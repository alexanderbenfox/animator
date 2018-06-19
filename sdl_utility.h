#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <string>

namespace SDLUtility {
  struct SDL_Deleter {
    void operator()(SDL_Window* window) const {
      SDL_DestroyWindow(window);
    }
    void operator()(SDL_Renderer* renderer) const {
      SDL_DestroyRenderer(renderer);
    }
    void operator()(SDL_Texture* texture) const {
      SDL_DestroyTexture(texture);
    }
    void operator()(SDL_Surface* surface) const {
      SDL_FreeSurface(surface);
    }
  };



  static std::unique_ptr<SDL_Renderer, SDL_Deleter> CreateUniqueRenderer(SDL_Window* window) {
    return std::unique_ptr<SDL_Renderer, SDL_Deleter>(
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED),
      SDL_Deleter()
      );
  }

  static std::shared_ptr<SDL_Window> CreateSharedWindow(const std::string& title, int x, int y, int w, int h, Uint32 flags) {
    return std::shared_ptr<SDL_Window>(
      SDL_CreateWindow(title.c_str(), x, y, w, h, flags),
      SDL_Deleter()
      );
  }

  static std::unique_ptr<SDL_Window, SDL_Deleter> CreateUniqueWindow(const std::string& title, int x, int y, int w, int h, Uint32 flags) {
    return std::unique_ptr<SDL_Window, SDL_Deleter>(
      SDL_CreateWindow(title.c_str(), x, y, w, h, flags),
      SDL_Deleter()
      );
  }

  static std::shared_ptr<SDL_Surface> CreateSharedSurface(const std::string& imgfile) {
    return std::shared_ptr<SDL_Surface>(
      IMG_Load(imgfile.c_str()),
      SDL_Deleter()
      );
  }
}