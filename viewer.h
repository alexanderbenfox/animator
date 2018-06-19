#pragma once
#include "spritesheet.h"

class AnimationViewer {
public:
  AnimationViewer() : _viewing(false), _frameIndex(0), _timeElapsed(0){}
  AnimationViewer(SDL_Renderer* renderer) : _viewing(false), _frameIndex(0), _timeElapsed(0), _renderer(renderer) {}

  void AddSheet(const std::string& file, const Vector2& spriteDimensions);
  void CreateBasicAnimation(const std::string& name);
  void CreateNewAnimation();
  void SetNewAnim(const std::string& animName);

  void UpdateView(float dt);
  void DrawView(float dt, const Vector2& renderLocation, const Vector2& renderSize);

private:
  int _frameIndex;
  float _timeElapsed;

  SDL_Renderer* _renderer;
  std::vector<SpriteSheet> _sheets;
  SpriteSheet _currentSheet;
  Animation _currentAnimation;

  bool _viewing;
};