#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct Wall {
    std::vector<sf::Vector2f> vertices;
};

class Map : public sf::Drawable {
  public:
    Map();
    ~Map() override = default;
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;
    Map(Map&&) noexcept = default;
    Map& operator=(Map&&) = delete;

    bool load(const std::string& jsonPath, const std::string& texturePath);

    const std::vector<Wall>& get_walls() const;
    const Wall& get_finish_line() const;
    const sf::Vector2f& get_player1_spawn() const;
    const sf::Vector2f& get_player2_spawn() const;

  protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    void parse_object_group_layer(const nlohmann::json& layer);
    void parse_point_object(const nlohmann::json& obj, float x, float y);
    void parse_polygon_object(const nlohmann::json& obj, const std::string& layer_name, float x,
                              float y);
    void parse_rectangle_object(const nlohmann::json& obj, const std::string& layer_name, float x,
                                float y);

    std::vector<Wall> walls;
    Wall finishLine;
    sf::Vector2f player1Spawn;
    sf::Vector2f player2Spawn;

    std::unique_ptr<sf::Sprite> backgroundSprite;
    sf::Texture backgroundTexture;
};

#endif // MAP_H