#include "bnMessageQuiz.h"

Quiz::Quiz(const std::string& optionA, const std::string& optionB, const std::string& optionC, const std::function<void(int)>& onResponse) :
  MessageInterface(("  " +optionA + "\n  " + optionB + "\n  " + optionC).c_str()),
  onResponse(onResponse) {
  selection = 0;
  options = {
    optionA,
    optionB,
    optionC
  };

  // select the first nonblank
  while (selection < 2) {
    if (options[selection] != "") {
      break;
    }

    selection += 1;
  }

  ResourceHandle handle;
  selectCursor.setTexture(handle.Textures().LoadFromFile(TexturePaths::TEXT_BOX_CURSOR));
  selectCursor.scale(2.0f, 2.0f);
}

/**
 * @brief Moves selection up if applicable
 * @return true if success, false otherwise
 */
const bool Quiz::SelectUp() {
  auto oldSelection = selection;

  while (selection > 0) {
    selection -= 1;

    // skip blank
    if (options[selection] == "") continue;

    return true;
  }

  // could not select anything
  selection = oldSelection;
  return false;
}

/**
 * @brief Moves selection down if applicable
 * @return true if success, false otherwise
 */
const bool Quiz::SelectDown() {
  auto oldSelection = selection;

  while (selection < 2) {
    selection += 1;

    // skip blank
    if (options[selection] == "") continue;

    return true;
  }

  // could not select anything
  selection = oldSelection;
  return false;
}

void Quiz::ConfirmSelection() {
  onResponse(selection);
  GetTextBox()->DequeMessage();
}

void Quiz::OnUpdate(double elapsed) {
}

void Quiz::OnDraw(sf::RenderTarget& target, sf::RenderStates states) {
  auto textbox = GetTextBox();

  textbox->DrawMessage(target, states);

  if (textbox->IsPlaying()) {
    return;
  }

  auto textboxPosition = textbox->getPosition();
  auto textboxBottom = textboxPosition.y - 45;

  auto cursorX = textboxPosition.x + 100 + 1;
  auto cursorY = textboxBottom + (float)selection * textbox->GetFont().GetLineHeight() * 2;

  selectCursor.setPosition(cursorX,  cursorY);

  target.draw(selectCursor, states);
}
