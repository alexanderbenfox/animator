#pragma once

#include "math.h"
#include "sdl_utility.h"
#include <map>
#include <vector>

struct Animation;

class SpriteSheet {
public:
  SpriteSheet() {}
  SpriteSheet(const std::string& file, SDL_Renderer* renderer, const Vector2& spriteDimension);

  void AddAnimation(const std::string name, float timeScale, int frames, Vector2 startPos, Vector2 offset);
  bool ContainsAnimation(const std::string& name);
  Animation GetAnimation(const std::string& name);

  int width() const {
    return _dimensions.x();
  }

  int height() const {
    return _dimensions.y();
  }

  SDL_Texture* texture() const {
    return _texture;
  }

  Vector2 _spriteDimensions;

private:
  Vector2 _dimensions;
  SDL_Texture* _texture;
  std::shared_ptr<SDL_Surface> _surface;

  std::map<std::string, Animation> _animations;
};

struct Animation {
public:
  std::string name;
  float timeScale;
  int numberOfFrames;
  Vector2 startPos;
  Vector2 dimensions;
  Vector2 offset;
  std::vector<SDL_Rect> frames;
};