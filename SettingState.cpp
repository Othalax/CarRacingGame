#include "SettingState.h"

SettingState::SettingState(sf::RenderWindow* window, std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states)
    : State(window, supportedKeys, states)
{
    this->initButtons();
    this->initCarTypes();

    this->player1view = new sf::Sprite(textures[State::player1car]);
    this->player1view->setPosition(sf::Vector2f(200.f, 200.f));
    this->player2view = new sf::Sprite(textures[State::player2car]);
    this->player2view->setPosition(sf::Vector2f(400.f, 200.f));
}

SettingState::~SettingState()
{
    //dtor
}

void SettingState::initButtons()
{
    this->buttons["menu"] = std::make_unique<Button>(window, 150.f, 150.f, 150.f, 50.f, "Main menu", textures["baseButton"], textures["baseButtonClicked"]);
    this->buttons["player1arrowright"] = std::make_unique<Button>(window, 250.f, 400.f, 30.f, 30.f, "", textures["arrowButtonRight"], textures["arrowButtonRightClicked"]);
    this->buttons["player1arrowleft"] = std::make_unique<Button>(window, 200.f, 400.f, 30.f, 30.f, "", textures["arrowButtonLeft"], textures["arrowButtonLeftClicked"]);
    this->buttons["player2arrowright"] = std::make_unique<Button>(window, 450.f, 400.f, 30.f, 30.f, "", textures["arrowButtonRight"], textures["arrowButtonRightClicked"]);
    this->buttons["player2arrowleft"] = std::make_unique<Button>(window, 400.f, 400.f, 30.f, 30.f, "", textures["arrowButtonLeft"], textures["arrowButtonLeftClicked"]);
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

void SettingState::updateButtons()
{
    if (this->buttons["menu"]->isPressed())
    {
        this->states->push_back(std::make_unique<MenuState>(this->window, supportedKeys, states));
    }

    if (this->buttons["player1arrowright"]->isPressed() && player1car != carTypes[carTypes.size() - 1])
    {
        State::player1car = carTypes[distance(carTypes.begin(), std::find(carTypes.begin(), carTypes.end(), State::player1car)) + 1];
        this->player1view->setTexture(textures[player1car]);
        this->buttons["player1arrowright"]->changeState();
    }

    if (this->buttons["player1arrowleft"]->isPressed() && player1car != carTypes[0])
    {
        State::player1car = carTypes[distance(carTypes.begin(), std::find(carTypes.begin(), carTypes.end(), State::player1car)) - 1];
        this->player1view->setTexture(textures[player1car]);
        this->buttons["player1arrowleft"]->changeState();
    }

    if (this->buttons["player2arrowright"]->isPressed() && player2car != carTypes[carTypes.size() - 1])
    {
        State::player2car = carTypes[distance(carTypes.begin(), std::find(carTypes.begin(), carTypes.end(), State::player2car)) + 1];
        this->player2view->setTexture(textures[player2car]);
        this->buttons["player2arrowright"]->changeState();
    }

    if (this->buttons["player2arrowleft"]->isPressed() && player2car != carTypes[0])
    {
        State::player2car = carTypes[distance(carTypes.begin(), std::find(carTypes.begin(), carTypes.end(), State::player2car)) - 1];
        this->player2view->setTexture(textures[player2car]);
        this->buttons["player2arrowleft"]->changeState();
    }
}

void SettingState::update(const float& dt)
{
    updateMousePos();
    for(auto &itr : this->buttons)
    {
        itr.second->update(mousePosView);
    }
    updateButtons();
}

void SettingState::render(sf::RenderTarget& target)
{
    for(auto &itr : this->buttons)
    {
        itr.second->render(target);
    }

    target.draw(*this->player1view);
    target.draw(*this->player2view);
}
