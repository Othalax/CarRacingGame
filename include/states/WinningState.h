#ifndef WINNINGSTATE_H
#define WINNINGSTATE_H

#include "Button.h"
#include "State.h"

class WinningState : public State {
  public:
    WinningState(sf::RenderWindow* window,
                 std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys, char winner);
    ~WinningState() override = default;
    WinningState(const WinningState&) = delete;
    WinningState& operator=(const WinningState&) = delete;
    WinningState(WinningState&&) noexcept = default;
    WinningState& operator=(WinningState&&) = delete;

    void update_buttons();
    void update(const float& /*dt*/) override;
    void render(sf::RenderTarget& target) override;

  private:
    Button mainMenu;
    Button playAgain;

    sf::Sprite background;

    sf::Text winningText;
    sf::Text scoresText;

    void init_scores(char winner);
};

#endif // WINNINGSTATE_H