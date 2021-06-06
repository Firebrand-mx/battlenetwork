#pragma once
#include <array>
#include <vector>
#include <map>
#include <functional>
#include <SFML/Window/Event.hpp>

using std::map;
using std::vector;

#include "bnInputEvent.h"
#include "bnConfigReader.h"
#include "bnConfigWriter.h"
#include "bnConfigSettings.h"

using std::map;
using std::vector;

/**
 * @class InputManager
 * @author mav
 * @date 01/05/19
 * @brief Handles SFML event polling, checks for valid controller configurations, and manages event states
 */
class InputManager {
public:

  /**
  * @brief sets all initial input events to false
  */
  InputManager(sf::Window& win);

  // no copies
  InputManager(const InputManager&) = delete;

  /**
   * @brief Frees config pointer
   */
  ~InputManager();
  
  /**
   * @brief Polls SFML's event queue used by windows and input devices
   * 
   * If controller is enabled, maps controller events to game input events
   * This is achieved through the configuration object that maps actions to paired
   * buttons.
   * 
   * Before this function ends, it will scan for any release events. If they are found,
   * they are cleared from the event list. If no release is found but a press is present,
   * the button state is transformed into a HELD state equivalent.
   */
  void Update();

  sf::Keyboard::Key GetAnyKey() const;
  
  std::string GetClipboard() const;
  void SetClipboard(const std::string& data);

  Gamepad GetAnyGamepadButton() const;
  const std::unordered_map<std::string, InputState>& StateThisFrame() const;
  
  const bool ConvertKeyToString(const sf::Keyboard::Key key, std::string& out) const;

  /**
   * @brief Queries if an input event has been fired
   * @param _event the event to look for.
   * @return true if present, false otherwise
   */
  bool Has(InputEvent _event) const;
  
  /**
   * @brief Checks if the input event list is empty
   * @return true if empty, false otherwise
   */
  bool Empty() const;
  
  /**
   * @brief Creates a reference to the config reader object
   * @param config reader
   */
  void SupportConfigSettings(ConfigReader& reader);
  
  /**
   * @brief Returns true if the config reader is set and config file is valid
   * @return 
   */
  bool IsConfigFileValid() const;
  
  /**
   * @brief Begins capturing entered text instead of firing game input events
   * 
   * Used for name or text entry
   */
  void BeginCaptureInputBuffer();
  
  /**
   * @brief Ends text capture state
   * 
   * Used when leaving input fields
   */
  void EndCaptureInputBuffer();

  /**
   * @brief Tells input manager to prioritize keyboard bindings
   */
  void UseKeyboardControls();

  /**
  * @brief Tells input manager to prioritize gamepad bindings
  */
  void UseGamepadControls();

  /**
  * @brief Tells input manager to use the gamepad at `index` in the list of gamepads
  * 
  * index must be >= 0 and < numOfGamepads
  */
  void UseGamepad(size_t index);

  /**
   * @brief Returns number of available gamepads
   * @return size_t
   */
  const size_t GetGamepadCount() const;
  
  /**
   * @brief Returns the contents of the current captured text
   * @return const std::string
   */
  const std::string GetInputBuffer() const;
  
  /**
   * @brief Transforms SFML keycodes into ASCII char texts and stores into input buffer
   * @param e input keycode event
   */
  void HandleInputBuffer(sf::Event e);
  
  /**
   * @brief Overloads the input buffer
   * @param buff text to modify
   * 
   * Used when selecting existing input fields
   * After being set, the buffer can be deleted with backspace or modified by typing
   */
  void SetInputBuffer(std::string buff);

  /**
   * @brief fires a key press manually
   * @param event key event to fire
   *
   * Used best on android where the virtual touch pad areas must map to keys
   */
  void VirtualKeyEvent(InputEvent event);

  /**
  * @brief binds function to invoke when regain focus event is fired 
  * @param callback the function to invoke
  */
  void BindRegainFocusEvent(std::function<void()> callback);

  /**
* @brief binds function to invoke when resized focus event is fired
* @param callback the function to invoke
*/
  void BindResizedEvent(std::function<void(int, int)> callback);

  /**
  * @brief binds function to invoke when regain lost focus event is fired
  * @param callback the function to invoke
  */
  void BindLoseFocusEvent(std::function<void()> callback);

  const bool IsGamepadAvailable() const;
  const bool HasSystemCopyEvent() const;
  const bool HasSystemPasteEvent() const;
  const bool IsUsingGamepadControls() const;
  const bool IsUsingKeyboardControls() const;

  ConfigSettings GetConfigSettings();

  /**
  * @brief if any buttons are held ore pressed, fire release events for all
  */
  void FlushAllInputEvents();

private:
  std::mutex mutex;
  sf::Keyboard::Key lastkey{};
  sf::Window& window;

  Gamepad lastButton{};

  bool captureInputBuffer{}; /*!< Flags input buffer capture state */
  std::string inputBuffer; /*!< The internal input buffer data */

  bool systemCopyEvent{ false }, systemPasteEvent{ false };
  bool hasFocus{ true };
  bool useGamepadControls{ true };
  bool useKeyboardControls{ true };

  std::vector<sf::Joystick::Identification> gamepads;
  unsigned int currGamepad{};

  std::array<bool, sf::Keyboard::KeyCount> keyboardState;
  std::unordered_map<unsigned int, bool> gamepadState;
  std::unordered_map<std::string, InputState> state; /*!< Current state */
  std::unordered_map<std::string, InputState> stateLastFrame; /*!< The state prior to this update */
  map<InputEvent, std::string> input; /*!< Maps controller events*/
  ConfigSettings settings; /*!< Settings object*/

  std::function<void()> onRegainFocus; /*!< How the application should respond to regaining focus */
  std::function<void()> onLoseFocus; /*!< How the application should respond to losing focus */
  std::function<void(int, int)> onResized; /*!< How the application should respond to resized */

  struct Binding {
    bool isKeyboardBinding{};
    unsigned int input{};
  };

  std::vector<std::pair<std::string, std::vector<Binding>>> bindings;
};
