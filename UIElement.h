#pragma once
#include "math.h"
#include <functional>
#include <string>
#include <vector>
#include <map>
#include "KW_editbox.h"

class GUIWindow;

class UIElement {
public:
  UIElement(Vector2 location, Vector2 size, GUIWindow* window, KW_GUI* gui);
  virtual ~UIElement();
  virtual GUIWindow* getWindow();

  virtual Vector2 getLocation();
  virtual Vector2 getSize();

protected:
  Vector2 _location;
  Vector2 _size;
  GUIWindow* _parentWindow;
  KW_GUI* _gui;
};

class Button : public UIElement {
public:
  Button(const std::string& text, KW_GUI* gui, KW_Widget* parent, const Vector2& location, const Vector2& size, GUIWindow* window);
  virtual ~Button();

  void SetOwner(std::shared_ptr<GUIWindow>& window);
  void SetOwner(std::shared_ptr<UIElement>& element);

  virtual void SetClickCallback(std::function<void(KW_Widget*, int)> fn);
  void SetButtonData(void* data);
  std::string GetButtonText();

  KW_Widget* GetButtonWidget() {
    return _button;
  }

  KW_Widget* GetLabelWidget() {
    return _label;
  }

protected:
  KW_Widget * _button;
  KW_Widget* _label;

  std::string _buttonText;
  std::function<void(KW_Widget*, int)> _btnDownFn;

  bool _ownerSet;
};

class EditBox : public UIElement {
public:
  EditBox(KW_GUI* gui, KW_Widget* parent, const Vector2& location, const Vector2& size, const std::vector<std::string> textBoxes, GUIWindow* window);
  ~EditBox();

  std::string GetText(const std::string& boxName);

  std::shared_ptr<Button> btn;

private:

  std::map<std::string, KW_Widget*> _boxes;

  std::string _buttonText;
  std::function<void(KW_Widget*, int)> _btnDownFn;
};

class DropDownButton : public Button {
public:
  DropDownButton(const std::string& text, KW_GUI* gui, KW_Widget* parent, const Vector2& location, const Vector2& size, GUIWindow* window);
  ~DropDownButton();
  void SetClickCallback(std::function<void(KW_Widget*, int)> fn) override;
  Button* AddButton(const std::string& text, std::shared_ptr<UIElement>& owner, std::function<void(KW_Widget*, int)> callback, void* data = nullptr);
private:
  std::map<std::string, Button*> _drop;
  Vector2 _lastLocation;
  std::unique_ptr<bool> _showing;
};