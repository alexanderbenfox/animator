#include "UIElement.h"
#include "guiwindow.h"


#pragma region UIElement

UIElement::UIElement(Vector2 location, Vector2 size, GUIWindow* window, KW_GUI* gui) : _location(location), _size(size), _parentWindow(window), _gui(gui) {}
UIElement::~UIElement() {}

GUIWindow* UIElement::getWindow() {
  return _parentWindow;
}

Vector2 UIElement::getLocation() {
  return _location;
}

Vector2 UIElement::getSize() {
  return _size;
}

#pragma endregion

#pragma region Button

Button::Button(const std::string& text, KW_GUI* gui, KW_Widget* parent, const Vector2& location, const Vector2& size, GUIWindow* window) : UIElement(location, size, window, gui) {
  KW_Rect btnGeometry;
  btnGeometry.x = location.x();
  btnGeometry.y = location.y();
  btnGeometry.w = size.x();
  btnGeometry.h = size.y();

  KW_Rect labelGeometry;
  labelGeometry.x = 0;
  labelGeometry.y = 0;
  labelGeometry.w = size.x();
  labelGeometry.h = size.y();

  _button = KW_CreateFrame(gui, parent, &btnGeometry);
  _buttonText = std::string(text);
  _label = KW_CreateLabel(gui, _button, _buttonText.c_str(), &labelGeometry);
}

Button::~Button() {
  KW_DestroyWidget(_button, 1);
}

void Button::SetButtonData(void* data) {
  if (data)
    KW_SetWidgetUserData(_label, data);
}

void Button::SetOwner(std::shared_ptr<GUIWindow>& window) {
  _ownerSet = true;
  KW_SetWidgetUserData(_label, (void*)&window);
}

void Button::SetOwner(std::shared_ptr<UIElement>& element) {
  _ownerSet = true;
  KW_SetWidgetUserData(_label, (void*)&element);
}

std::string Button::GetButtonText() {
  return _buttonText;
}

void Button::SetClickCallback(std::function<void(KW_Widget*, int)> fn) {
  if (_ownerSet) {
    _btnDownFn = std::function<void(KW_Widget*, int)>(fn);
    void(decltype(_btnDownFn)::*ptr)(KW_Widget*, int) const = &decltype(_btnDownFn)::operator();
    auto func_ptr = *_btnDownFn.target<void(*)(KW_Widget*, int)>();
    KW_AddWidgetMouseDownHandler(_label, func_ptr);
  }
  else
    throw std::exception("Memory owner for button is not set\n");
}

#pragma endregion

#pragma region EditBox

EditBox::EditBox(KW_GUI* gui, KW_Widget* parent, const Vector2& location, const Vector2& size, const std::vector<std::string> textBoxes, GUIWindow* window) : UIElement(location, size, window, gui) {
  KW_Rect editBoxGeometry;
  editBoxGeometry.x = location.x();
  editBoxGeometry.y = location.y();
  editBoxGeometry.w = size.x();
  editBoxGeometry.h = size.y();

  Vector2 btnSize = Vector2(size.y(), size.y());
  Vector2 btnLocation = Vector2(location.x() + size.x() + btnSize.x() / 2, location.y());

  btn = std::make_shared<Button>("Submit", gui, parent, btnLocation, btnSize, window);

  for (auto it = textBoxes.begin(); it != textBoxes.end(); it++) {
    KW_Widget* box = KW_CreateEditbox(gui, parent, it->c_str(), &editBoxGeometry);
    _boxes.insert(std::make_pair(*it, box));
    //move location of next editbox
    editBoxGeometry.y -= editBoxGeometry.h;
  }
}

EditBox::~EditBox() {
  btn.reset();
  for (auto it = _boxes.begin(); it != _boxes.end(); it++) {
    KW_DestroyWidget(it->second, 1);
  }
}

std::string EditBox::GetText(const std::string& boxName) {
  const char* text = KW_GetEditboxText(_boxes[boxName]);
  return std::string(text);
}

#pragma endregion

#pragma region DropDownButton

namespace Callbacks {
  void ShowHideChildren(KW_Widget* widget, int button) {
    unsigned int count;
    auto children = KW_GetWidgetChildren(widget, &count);
    bool* showing = (bool*)KW_GetWidgetUserData(widget);
    for (int i = 0; i < count; i++) {
      if (*showing)
        KW_HideWidget(children[i]);
      else
        KW_ShowWidget(children[i]);
    }
    *showing = !(*showing);
  }
}


DropDownButton::DropDownButton(const std::string& text, KW_GUI* gui, KW_Widget* parent, const Vector2& location, const Vector2& size, GUIWindow* window) : Button(text, gui, parent, location, size, window) {
  _showing = std::make_unique<bool>(false);
  KW_SetWidgetUserData(_label, _showing.get());
  *_showing = false;
  SetClickCallback(Callbacks::ShowHideChildren);
}

DropDownButton::~DropDownButton() {
  for (auto btn : _drop) {
    delete btn.second;
  }
}

void DropDownButton::SetClickCallback(std::function<void(KW_Widget*, int)> fn) {
  _btnDownFn = std::function<void(KW_Widget*, int)>(fn);
  void(decltype(_btnDownFn)::*ptr)(KW_Widget*, int) const = &decltype(_btnDownFn)::operator();
  auto func_ptr = *_btnDownFn.target<void(*)(KW_Widget*, int)>();
  KW_AddWidgetMouseDownHandler(_label, func_ptr);
}



Button* DropDownButton::AddButton(const std::string& text, std::shared_ptr<UIElement>& owner, std::function<void(KW_Widget*, int)> callback, void* data) {
  Vector2 loc;
  Vector2 size = this->getSize();
  if (_drop.size() > 0)
    loc = Vector2(0, _lastLocation.y() + size.y());
  else
    loc = Vector2(0, size.y());

  Button* btn = new Button(text, _gui, this->_label, loc, size, this->getWindow());
  btn->SetOwner(owner);
  btn->SetClickCallback(callback);

  if (!(*_showing))
    KW_HideWidget(btn->GetButtonWidget());

  if (data)
    KW_SetWidgetUserData(btn->GetLabelWidget(), data);

  _drop.emplace(std::make_pair(text, btn));
  _lastLocation = loc;
  return btn;
}

#pragma endregion