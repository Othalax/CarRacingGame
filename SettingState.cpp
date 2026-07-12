#include "SettingState.h"
#include "MenuState.h"
#include "Config.h"

SettingState::SettingState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
                            sf::Keyboard::Key> supportedKeys)
    : State(window, supportedKeys),
	player1view(textures[State::player1car]), 
    player2view(textures[State::player2car]),
	background(textures[Config::instance().getSettingsBackground()]),
    mapText(font, Config::instance().getSettingsMapText().text, Config::instance().getSettingsMapText().fontSize),
	mapName(font, State::maps[State::currentMap].name, Config::instance().getSettingsMapName().fontSize)
{
    this->initButtons();

    this->carTypes = Config::instance().getCarTypes();

    const auto player1Layout = Config::instance().getSettingsPlayer1View();
    const auto player2Layout = Config::instance().getSettingsPlayer2View();
    const auto mapTextLayout = Config::instance().getSettingsMapText();
    const auto mapNameLayout = Config::instance().getSettingsMapName();
    const float logicalWidth = Config::instance().getLogicalSize().x;

    this->player1view.setPosition(sf::Vector2f(player1Layout.x, player1Layout.y));
	this->player1view.setScale(sf::Vector2f(player1Layout.scale, player1Layout.scale));
	this->player1view.rotate(sf::degrees(player1Layout.rotation));
    this->player2view.setPosition(sf::Vector2f(player2Layout.x, player2Layout.y));
	this->player2view.setScale(sf::Vector2f(player2Layout.scale, player2Layout.scale));
	this->player2view.rotate(sf::degrees(player2Layout.rotation));

    this->mapText.setPosition(sf::Vector2f((logicalWidth - this->mapText.getLocalBounds().size.x) / 2.f, mapTextLayout.y));
	this->mapName.setPosition(sf::Vector2f((logicalWidth - this->mapName.getLocalBounds().size.x) / 2.f, mapNameLayout.y));
}

void SettingState::initButtons()
{
    const auto& config = Config::instance();

    auto addButton = [this, &config](const std::string& name,
        const std::string& defaultTexture, const std::string& hoverTexture) {
        const auto layout = config.getSettingsButton(name);
        this->buttons.try_emplace(
            name,
            layout.x, layout.y, layout.width, layout.height, layout.text,
            textures[defaultTexture], textures[hoverTexture], font
        );
    };

    addButton("menu", "baseButton", "baseButtonClicked");
    addButton("player1arrowright", "arrowButtonRight", "arrowButtonRightClicked");
    addButton("player1arrowleft", "arrowButtonLeft", "arrowButtonLeftClicked");
    addButton("player2arrowright", "arrowButtonRight", "arrowButtonRightClicked");
    addButton("player2arrowleft", "arrowButtonLeft", "arrowButtonLeftClicked");
    addButton("resetScore", "baseButton", "baseButtonClicked");
    addButton("maparrowright", "arrowButtonRight", "arrowButtonRightClicked");
    addButton("maparrowleft", "arrowButtonLeft", "arrowButtonLeftClicked");
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
		this->mapName.setString(State::maps[State::currentMap].name);
        this->mapName.setPosition(sf::Vector2f(
            (Config::instance().getLogicalSize().x - this->mapName.getLocalBounds().size.x) / 2.f,
            Config::instance().getSettingsMapName().y));
    };

    if (this->buttons.at("menu").isPressed()) {
        this->nextState = std::make_unique<MenuState>(this->window, this->supportedKeys);
        return;
    }
    else if (this->buttons.at("resetScore").isPressed()) {
        Config::instance().resetScores();
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
