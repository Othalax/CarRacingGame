#ifndef MENUSTATE_H
#define MENUSTATE_H

#include <Game.h>
#include <GameState.h>
#include <Button.h>
#include <SettingState.h>

class MenuState
    : public State
{
    public:
        MenuState(sf::RenderWindow* window, std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states);
        virtual ~MenuState();

        void updateButtons();
        void update(const float& dt);
        void render(sf::RenderTarget& target);

    protected:
        Button* gamestate;
        Button* exit;
        Button* settings;
        sf::RectangleShape wiwiwi;

    private:
};

#endif // MENUSTATE_H
