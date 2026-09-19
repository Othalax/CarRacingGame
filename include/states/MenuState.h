#ifndef MENUSTATE_H
#define MENUSTATE_H

#include "Button.h"
#include "State.h"

class MenuState : public State {
  public:
    MenuState(sf::RenderWindow* window,
              std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys);
    ~MenuState() override = default;
    MenuState(const MenuState&) = delete;
    MenuState& operator=(const MenuState&) = delete;
    MenuState(MenuState&&) = default;
    MenuState& operator=(MenuState&&) = delete;

    void update_buttons();
    void update(const float& dt) override;
    void render(sf::RenderTarget& target) override;

  private:
    Button gamestate;
    Button settings;
    Button exit;

    sf::Sprite background;
};

#endif // MENUSTATE_H