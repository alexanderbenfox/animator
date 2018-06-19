#pragma once

#include <functional>
#include <vector>
#include "guiwindow.h"
#include "UIElement.h"



class ImportSheet : public GUIWindow {
public:
  ImportSheet() {}
  ImportSheet(std::shared_ptr<GUIWindow> parent, const Vector2& location, const Vector2& dimensions);
  //std::shared_ptr<Button> btn;
  std::shared_ptr<UIElement> _editBox;
  std::shared_ptr<UIElement> _dropDown;
};