#include "importsheet.h"
#include "util.h"
#include "window.h"

void DeleteButton(KW_Widget* widget, int button) {
  std::shared_ptr<EditBox>* btn = (std::shared_ptr<EditBox>*)KW_GetWidgetUserData(widget);
  btn->reset();
}

void SetAnimationFromList(KW_Widget* widget, int button) {
  Button* btn = (Button*)KW_GetWidgetUserData(widget);
  btn->getWindow()->GetMainWindow()->GetViewer().SetNewAnim(btn->GetButtonText());
}

void PressSubmit(KW_Widget* widget, int button) {
  std::shared_ptr<EditBox>* editBox = (std::shared_ptr<EditBox>*)KW_GetWidgetUserData(widget);
  std::string filePath = (*editBox)->GetText("File Location");
  if (FileUtils::FileExists(filePath)) {
    int w = std::stoi((*editBox)->GetText("Sprite Width"));
    int h = std::stoi((*editBox)->GetText("Sprite Height"));
    //add sheet to the viewer
    (*editBox)->getWindow()->GetMainWindow()->GetViewer().AddSheet(filePath, Vector2(w, h));
    
    //create a new entry on dropdown
    auto filename = StringUtils::Split(filePath, "\\").back();
    auto window = dynamic_cast<ImportSheet*>((*editBox)->getWindow());
    //create a new animation from the sliced up sheet
    (*editBox)->getWindow()->GetMainWindow()->GetViewer().CreateBasicAnimation(filename);
    auto btn = std::dynamic_pointer_cast<DropDownButton>(window->_dropDown)->AddButton(filename, window->_dropDown, SetAnimationFromList);
    btn->SetButtonData((void*)btn);
  }
}

ImportSheet::ImportSheet(std::shared_ptr<GUIWindow> parent, const Vector2& location, const Vector2& dimensions) : GUIWindow(parent, location, dimensions, "resources\\tileset\\tileset.png") {
  std::vector<std::string> variableNames = { "File Location", "Sprite Width", "Sprite Height" };
  _editBox = std::shared_ptr<UIElement>(new EditBox(_gui, _frame, Vector2(100, 100), Vector2(100, 40), variableNames, this));
  std::shared_ptr<EditBox> ptr = std::dynamic_pointer_cast<EditBox>(_editBox);
  ptr->btn->SetOwner(_editBox);
  ptr->btn->SetClickCallback(PressSubmit);

  _dropDown = std::shared_ptr<UIElement>(new DropDownButton("Imported Sheets", _gui, _frame, Vector2(300, 100), Vector2(100, 40), this));


}