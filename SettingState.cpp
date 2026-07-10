#include "SettingState.h"
#include "MenuState.h"

SettingState::SettingState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                            sf::Keyboard::Key> supportedKeys)
    : State(window, supportedKeys),
	player1view(textures[State::player1car]), 
    player2view(textures[State::player2car]),
	background(textures["settingsBackground"]),
    mapText(font, "Map:", 20),
	mapName(font, std::get<0>(State::maps[State::currentMap]), 10)
{
    this->initButtons();
    this->initCarTypes();

    this->player1view.setPosition(sf::Vector2f(292.5f, 70.f));
	this->player1view.setScale(sf::Vector2f(0.5f, 0.5f));
	this->player1view.rotate(sf::degrees(90.f));
    this->player2view.setPosition(sf::Vector2f(392.5f, 70.f));
	this->player2view.setScale(sf::Vector2f(0.5f, 0.5f));
	this->player2view.rotate(sf::degrees(90.f));

    this->mapText.setPosition(sf::Vector2f((640.f - this->mapText.getLocalBounds().size.x) / 2.f, 250.f));
	this->mapName.setPosition(sf::Vector2f((640.f - this->mapName.getLocalBounds().size.x) / 2.f, 290.f));
}

void SettingState::initButtons()
{
    this->buttons.try_emplace("menu", 282.5f, 30.f, 75.f, 25.f, "Main menu", textures["baseButton"], textures["baseButtonClicked"], font);
    this->buttons.try_emplace("player1arrowright", 277.5f, 160.f, 30.f, 30.f, "", textures["arrowButtonRight"], textures["arrowButtonRightClicked"], font);
    this->buttons.try_emplace("player1arrowleft", 232.5f, 160.f, 30.f, 30.f, "", textures["arrowButtonLeft"], textures["arrowButtonLeftClicked"], font);
    this->buttons.try_emplace("player2arrowright", 377.5f, 160.f, 30.f, 30.f, "", textures["arrowButtonRight"], textures["arrowButtonRightClicked"], font);
    this->buttons.try_emplace("player2arrowleft", 332.5f, 160.f, 30.f, 30.f, "", textures["arrowButtonLeft"], textures["arrowButtonLeftClicked"], font);
    this->buttons.try_emplace("resetScore", 277.5f, 210.f, 85.f, 25.f, "Reset scores", textures["baseButton"], textures["baseButtonClicked"], font);
    this->buttons.try_emplace("maparrowright", 377.5f, 280.f, 30.f, 30.f, "", textures["arrowButtonRight"], textures["arrowButtonRightClicked"], font);
    this->buttons.try_emplace("maparrowleft", 232.5f, 280.f, 30.f, 30.f, "", textures["arrowButtonLeft"], textures["arrowButtonLeftClicked"], font);
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
            }
            else if (newIndex < 0) {
                currentCar = this->carTypes.back();
                if (currentCar == other) {
                    currentCar = this->carTypes[this->carTypes.size() - 2];
				}
            }
            else if (newIndex >= static_cast<int>(this->carTypes.size())) {
                currentCar = this->carTypes.front();
                if (currentCar == other) {
                    currentCar = this->carTypes[1];
                }
			}
            view.setTexture(this->textures[currentCar]);
        }
    };

    auto changeMap = [this](int direction) {
        int newIndex = State::currentMap + direction;
            
        if (newIndex >= 0 && newIndex < static_cast<int>(this->maps.size())) {
            State::currentMap = newIndex;
        }
        else if (newIndex < 0) {
            State::currentMap = State::maps.size()-1;
        }
        else if (newIndex >= static_cast<int>(this->maps.size())) {
            State::currentMap = 0;
        }
		this->mapName.setString(std::get<0>(State::maps[State::currentMap]));
        this->mapName.setPosition(sf::Vector2f((640.f - this->mapName.getLocalBounds().size.x) / 2.f, 290.f));
    };

    if (this->buttons.at("menu").isPressed()) {
        this->nextState = std::make_unique<MenuState>(this->window, this->supportedKeys);
        return;
    }
    else if (this->buttons.at("resetScore").isPressed()) {
        std::ofstream ofs("config/scores.ini");

        ofs.clear();
        ofs << 0 << '\n';
        ofs << 0;

        ofs.close();
    }
    else if (this->buttons.at("player1arrowright").isPressed()) {
        changeCar(State::player1car, this->player1view, 1, State::player2car);
    }
    else if (this->buttons.at("player1arrowleft").isPressed()) {
        changeCar(State::player1car, this->player1view, -1, State::player2car);
    }

    else if (this->buttons.at("player2arrowright").isPressed()) {
        changeCar(State::player2car, this->player2view, 1, State::player1car);
    }
    else if (this->buttons.at("player2arrowleft").isPressed()) {
        changeCar(State::player2car, this->player2view, -1, State::player1car);
    }
    else if (this->buttons.at("maparrowright").isPressed()) {
        changeMap(1);
    }
    else if (this->buttons.at("maparrowleft").isPressed()) {
        changeMap(-1);
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
	target.draw(this->background, sf::RenderStates::Default);

    for(auto &itr : this->buttons)
    {
        itr.second.render(target);
    }

    target.draw(this->player1view);
    target.draw(this->player2view);

	target.draw(this->mapText);
	target.draw(this->mapName);
}
