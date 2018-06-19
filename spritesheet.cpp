#include "spritesheet.h"

SpriteSheet::SpriteSheet(const std::string& file, SDL_Renderer* renderer, const Vector2& spriteDimensions) {
  _surface = SDLUtility::CreateSharedSurface(file);
  _texture = SDL_CreateTextureFromSurface(renderer, _surface.get());
  _dimensions = Vector2((_surface->w), (_surface->h));
  _spriteDimensions = spriteDimensions;
}

void SpriteSheet::AddAnimation(const std::string name, float timeScale, int frames, Vector2 startPos, Vector2 offset) {
  Animation newAnim;
  std::vector<SDL_Rect> anim_frames;
  for (int i = 0; i < frames; i++) {
    SDL_Rect newRect = { (i + startPos.x()) * _spriteDimensions.x(), startPos.y(), _spriteDimensions.x(), _spriteDimensions.y()};
    newAnim.frames.push_back(newRect);
  }
  newAnim.name = name;
  newAnim.timeScale = timeScale;
  newAnim.numberOfFrames = frames;
  newAnim.startPos = Vector2(startPos);
  newAnim.dimensions = Vector2(_spriteDimensions);
  newAnim.offset = Vector2(offset);

  _animations.insert(std::make_pair(name, newAnim));
}

bool SpriteSheet::ContainsAnimation(const std::string& name) {
  return _animations.find(name) != _animations.end();
}

Animation SpriteSheet::GetAnimation(const std::string& name) {
  return _animations[name];
}