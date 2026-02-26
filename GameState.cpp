#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, std::unordered_map<std::string,sf::Keyboard::Key> supportedKeys, std::vector<std::unique_ptr<State>>* states)
    :State (window, supportedKeys, states)
{
    if(State::player1car == "")
    {
        State::player1car = "pinkCar";
    }
    std::unordered_map<std::string,sf::Keyboard::Key> keys;
    keys.insert({"forward", sf::Keyboard::Key::W});
    keys.insert({"backward", sf::Keyboard::Key::S});
    keys.insert({"left", sf::Keyboard::Key::A});
    keys.insert({"right", sf::Keyboard::Key::D});
    player1 = new Car(keys, textures[player1car], 200.f, 200.f, 0.f, 100.f, 30.f, 200.f);
    if(State::player2car == "")
    {
        State::player2car = "blueCar";
    }
    std::unordered_map<std::string,sf::Keyboard::Key> keys2;
    keys2.insert({"forward", sf::Keyboard::Key::Up});
    keys2.insert({"backward", sf::Keyboard::Key::Down});
    keys2.insert({"left", sf::Keyboard::Key::Left});
    keys2.insert({"right", sf::Keyboard::Key::Right});
    player2 = new Car(keys2, textures[player2car], 200.f, 200.f, 0.f, 100.f, 30.f, 200.f);
}

GameState::~GameState()
{
    //dtor
}

void GameState::update(const float& dt){
    this->player1->update(dt);
    this->player2->update(dt);
}

void GameState::render(sf::RenderTarget& target){
    this->player1->render(*this->window);
    this->player2->render(*this->window);
}
