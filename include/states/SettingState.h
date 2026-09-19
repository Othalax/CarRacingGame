#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include "Button.h"
#include "State.h"

#include <string>

class SettingState : public State {
  public:
    SettingState(sf::RenderWindow* window,
                 std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys);
    ~SettingState() override = default;
    SettingState(const SettingState&) = delete;
    SettingState& operator=(const SettingState&) = delete;
    SettingState(SettingState&&) noexcept = default;
    SettingState& operator=(SettingState&&) = delete;

    void update_buttons();
    void update(const float& dt) override;
    void render(sf::RenderTarget& target) override;

  private:
    sf::Sprite background;

    std::unordered_map<std::string, Button> buttons;
    std::vector<std::string> carTypes;

    sf::Sprite player1view;
    sf::Sprite player2view;

    sf::Text mapText;
    sf::Text mapName;

    void init_buttons();

    void change_car(std::string& currentCar, sf::Sprite& view, int direction,
                    const std::string& other);
    void change_map(int direction);
};

#endif // SETTINGSTATE_H