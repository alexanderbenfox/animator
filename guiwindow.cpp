#include "guiwindow.h"
#include "window.h"

GUIWindow::GUIWindow(SDL_Renderer* renderer, SDL_Window* window, MainWindow* mainWindow ) : _renderer(renderer), _window(window), _theWindow(mainWindow) {
  GUIObjectManager::getInstance().AddGUI(this);
  _driver = KW_CreateSDL2RenderDriver(renderer, window);

  _tileset = KW_LoadSurface(_driver, "resources\\tileset\\tileset.png");
  _gui = KW_Init(_driver, _tileset);
  _font = KW_LoadFont(_driver, "resources\\fonts\\Fontin-Regular.ttf", 12);

  KW_SetFont(_gui, _font);

  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  _windowLocation = Vector2();
  _frameSize = Vector2(w, h);

  KW_Rect geometry;
  geometry.x = 0;
  geometry.y = 0;
  geometry.w = w;
  geometry.h = h;

  _frame = KW_CreateFrame(_gui, nullptr, &geometry);
}

GUIWindow::GUIWindow(std::shared_ptr<GUIWindow> parent, const Vector2& location, const Vector2& dimensions, const std::string& tilesetFile) : _parent(parent), _windowLocation(Vector2(location)), _frameSize(Vector2(dimensions)) {
  GUIObjectManager::getInstance().AddGUI(this);
  _tileset = KW_LoadSurface(parent->GetDriver(), tilesetFile.c_str());
  _gui = KW_Init(parent->GetDriver(), _tileset);
  _font = KW_LoadFont(parent->GetDriver(), "resources\\fonts\\Fontin-Regular.ttf", 12);

  KW_SetFont(_gui, _font);

  KW_Rect geometry;
  geometry.x = location.x();
  geometry.y = location.y();
  geometry.w = dimensions.x();
  geometry.h = dimensions.y();

  _frame = KW_CreateFrame(_gui, nullptr, &geometry);
}

GUIWindow::~GUIWindow() {
  KW_Quit(_gui);
  if(_driver && _font)
    KW_ReleaseFont(_driver, _font);
  if(_driver && _tileset)
    KW_ReleaseSurface(_driver, _tileset);
  if(_driver)
    KW_ReleaseRenderDriver(_driver);
}

void GUIWindow::draw() {
  KW_ProcessEvents(_gui);
  KW_Paint(_gui);
}

