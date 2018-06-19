#include "viewer.h"

void AnimationViewer::AddSheet(const std::string& file, const Vector2& spriteDimensions) {
  _sheets.push_back(SpriteSheet(file, _renderer, spriteDimensions));
}

void AnimationViewer::CreateBasicAnimation(const std::string& name) {
  //create anim out of all of the images for now
  int totalFrames = _sheets.back().width() / _sheets.back()._spriteDimensions.x() * _sheets.back().height() / _sheets.back()._spriteDimensions.y();
  _sheets.back().AddAnimation(name, 100, totalFrames, Vector2(), Vector2());
  SetNewAnim(name);
}

void AnimationViewer::SetNewAnim(const std::string& animName) {
  if (!_currentSheet.ContainsAnimation(animName)) {
    for (auto& sheet : _sheets) {
      if (sheet.ContainsAnimation(animName)) {
        _currentSheet = sheet;
        break;
      }
    }
  }
  _currentAnimation = _currentSheet.GetAnimation(animName);
  _frameIndex = 0;
  _timeElapsed = 0;
  _viewing = true;
}


void AnimationViewer::UpdateView(float dt) {
  if (_viewing) {
    _timeElapsed += dt;
    if (_timeElapsed > _currentAnimation.timeScale) {
      float time = _timeElapsed;
      _timeElapsed -= _currentAnimation.timeScale;
      if (_frameIndex < _currentAnimation.numberOfFrames - 1)
      {
        int frameJump = time / _currentAnimation.timeScale;
        int framesLeft = (_currentAnimation.numberOfFrames - 1) - _frameIndex;
        if (frameJump > framesLeft) {
          frameJump = framesLeft;
        }
        _frameIndex += frameJump;
      }
      else
      {
        _frameIndex = 0;
      }
    }
  }
}

void AnimationViewer::DrawView(float dt, const Vector2& renderLocation, const Vector2& renderSize) {
  if (_viewing) {
    SDL_Rect dstRect;
    dstRect.x = renderLocation.x() + _currentAnimation.offset.x();
    dstRect.y = renderLocation.y() + _currentAnimation.offset.y();
    dstRect.w = renderSize.x(); //_currentSheet.width();
    dstRect.h = renderSize.y(); // _currentSheet.height();

    SDL_Rect srcRect = _currentAnimation.frames[_frameIndex];
    int w, h;

    try {
      if (SDL_QueryTexture(_currentSheet.texture(), NULL, NULL, &w, &h) == 0)
        SDL_RenderCopy(_renderer, _currentSheet.texture(), &srcRect, &dstRect);
    }
    catch (const std::exception& e) {
      printf("Texture is not valid.\n");
    }
  }
}