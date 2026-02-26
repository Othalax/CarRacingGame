#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include <MenuState.h>
#include <Button.h>

class SettingState
    : public State
{
    public:
        SettingState(sf::RenderWindow* window, std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states);
        virtual ~SettingState();

        void updateButtons();
        void update(const float& dt);
        void render(sf::RenderTarget& target);
    protected:
        std::unordered_map<std::string, std::unique_ptr<Button>> buttons;
        std::vector<std::string> carTypes;

        sf::Sprite* player1view;
        sf::Sprite* player2view;

        void initButtons();
        void initCarTypes();

    private:
};

#endif // SETTINGSTATE_H
