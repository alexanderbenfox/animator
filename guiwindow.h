#pragma once

#include <memory>
#include <string>
#include "math.h"
#include "KW_gui.h"
#include "KW_frame.h"
#include "KW_label.h"
#include "KW_renderdriver_sdl2.h"

class MainWindow;

class GUIWindow {
public:
  GUIWindow() {};
  GUIWindow(SDL_Renderer* renderer, SDL_Window* window, MainWindow* mainWindow);
  GUIWindow(std::shared_ptr<GUIWindow> parent, const Vector2& location, const Vector2& dimensions, const std::string& tilesetFile);

  virtual ~GUIWindow();

  virtual KW_RenderDriver* GetDriver() { 
    if (_driver) return _driver;
    else return _parent->GetDriver();
  }
  virtual SDL_Renderer* GetRenderer() {
    if (_renderer) return _renderer;
    else return _parent->GetRenderer();
  }
  virtual SDL_Window* GetWindow() {
    if (_window) return _window;
    else return _parent->GetWindow();
  }

  virtual MainWindow* GetMainWindow() {
    if (_theWindow) return _theWindow;
    else return _parent->GetMainWindow();
  }

  virtual void setGID(int gid) {
    _gid = gid;
  }

  virtual int getGID() {
    return _gid;
  }

  virtual void draw();

protected:
  KW_GUI* _gui;
  KW_Surface* _tileset;
  KW_Font* _font;
  KW_Widget* _frame;

  std::shared_ptr<GUIWindow> _parent;
  Vector2 _windowLocation;
  Vector2 _frameSize;

  int _gid;

private:
  KW_RenderDriver * _driver;
  SDL_Renderer* _renderer;
  SDL_Window* _window;
  MainWindow* _theWindow;
};

#include <map>
class GUIObjectManager {
public:
  static GUIObjectManager& getInstance() {
    static GUIObjectManager instance;
    return instance;
  }

  void AddGUI(GUIWindow* guiObj) {
    guiObj->setGID(_currentGID);
    _objectMap.insert(std::make_pair(_currentGID, guiObj));
    _currentGID++;
  }

  GUIWindow* GetGUI(int gid) {
    return _objectMap[gid];
  }

private:
  GUIObjectManager(){}
  GUIObjectManager(GUIObjectManager const&) = delete;
  void operator=(GUIObjectManager const&) = delete;

  int _currentGID = 0;
  std::map<int, GUIWindow*> _objectMap;
};