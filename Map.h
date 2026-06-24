#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <array>

struct Wall {
    std::vector<sf::Vector2f> vertices; 
};

class Map : public sf::Drawable {
public:
    Map();
    ~Map() = default;

    bool load(const std::string& jsonPath, const std::string& texturePath);

    const std::vector<Wall>& getWalls() const;
    const Wall& getStartLine() const;
    const Wall& getFinishLine() const;
    const sf::Vector2f& getPlayer1Spawn() const;
    const sf::Vector2f& getPlayer2Spawn() const;

private:
    std::vector<Wall> walls;
    Wall startLine;
    Wall finishLine;
    sf::Vector2f player1Spawn;
    sf::Vector2f player2Spawn;

    std::unique_ptr<sf::Sprite> backgroundSprite;
    sf::Texture backgroundTexture;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};