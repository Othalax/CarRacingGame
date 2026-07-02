#include "SettingState.h"
#include "MenuState.h"

SettingState::SettingState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                            sf::Keyboard::Key> supportedKeys)
    : State(window, supportedKeys),
	player1view(textures[State::player1car]), 
    player2view(textures[State::player2car])
{
    this->initButtons();
    this->initCarTypes();

    player1view.setPosition(sf::Vector2f(200.f, 200.f));
    player2view.setPosition(sf::Vector2f(400.f, 200.f));
}

void SettingState::initButtons()
{
    this->buttons.try_emplace("menu", 150.f, 150.f, 150.f, 50.f, "Main menu", textures["baseButton"], textures["baseButtonClicked"], font);
    this->buttons.try_emplace("player1arrowright", 250.f, 400.f, 30.f, 30.f, "", textures["arrowButtonRight"], textures["arrowButtonRightClicked"], font);
    this->buttons.try_emplace("player1arrowleft", 200.f, 400.f, 30.f, 30.f, "", textures["arrowButtonLeft"], textures["arrowButtonLeftClicked"], font);
    this->buttons.try_emplace("player2arrowright", 450.f, 400.f, 30.f, 30.f, "", textures["arrowButtonRight"], textures["arrowButtonRightClicked"], font);
    this->buttons.try_emplace("player2arrowleft", 400.f, 400.f, 30.f, 30.f, "", textures["arrowButtonLeft"], textures["arrowButtonLeftClicked"], font);
}

void SettingState::initCarTypes()
{
    std::ifstream ifs("config/cars.ini");
    std::string carType;

    if (ifs.is_open())
    {
        while (ifs >> carType)
        {
            this->carTypes.push_back(carType);
        }
    }
    ifs.close();
}

void SettingState::updateButtons() {
    if (this->buttons.at("menu").isPressed()) {
        this->nextState = std::make_unique<MenuState>(this->window, this->supportedKeys);
        return;
    }

    auto changeCar = [this](std::string& currentCar, sf::Sprite& view, int direction, std::string& other) {
        auto it = std::find(this->carTypes.begin(), this->carTypes.end(), currentCar);
        auto itOther = std::find(this->carTypes.begin(), this->carTypes.end(), other);
        if (it != this->carTypes.end()) {
            int currentIndex = static_cast<int>(std::distance(this->carTypes.begin(), it));
            int currentIndexOther = static_cast<int>(std::distance(this->carTypes.begin(), itOther));
            int newIndex = currentIndex + direction;
            if (newIndex == currentIndexOther) {
                newIndex += direction;
            }

            if (newIndex >= 0 && newIndex < static_cast<int>(this->carTypes.size())) {
                currentCar = this->carTypes[newIndex];
                view.setTexture(this->textures[currentCar]);
            }
            else if (newIndex < 0) {
                currentCar = this->carTypes.back();
                view.setTexture(this->textures[currentCar]);
            }
            else if (newIndex >= static_cast<int>(this->carTypes.size())) {
                currentCar = this->carTypes.front();
                view.setTexture(this->textures[currentCar]);
			}
        }
        };

    if (this->buttons.at("player1arrowright").isPressed()) {
        changeCar(State::player1car, this->player1view, 1, State::player2car);
        this->buttons.at("player1arrowright").changeState();
    }
    if (this->buttons.at("player1arrowleft").isPressed()) {
        changeCar(State::player1car, this->player1view, -1, State::player2car);
        this->buttons.at("player1arrowleft").changeState();
    }

    if (this->buttons.at("player2arrowright").isPressed()) {
        changeCar(State::player2car, this->player2view, 1, State::player1car);
        this->buttons.at("player2arrowright").changeState();
    }
    if (this->buttons.at("player2arrowleft").isPressed()) {
        changeCar(State::player2car, this->player2view, -1, State::player1car);
        this->buttons.at("player2arrowleft").changeState();
    }
}

void SettingState::update(const float& dt)
{
    updateMousePos();
    for(auto &itr : this->buttons)
    {
        itr.second.update(mousePosView);
    }
    updateButtons();
}

void SettingState::render(sf::RenderTarget& target)
{
    for(auto &itr : this->buttons)
    {
        itr.second.render(target);
    }

    target.draw(this->player1view);
    target.draw(this->player2view);
}
