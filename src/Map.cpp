#include "Map.h"

#include <fstream>

Map::Map() {
    this->player1Spawn = {100.F, 100.F};
    this->player2Spawn = {150.F, 100.F};
}

bool Map::load(const std::string& jsonPath, const std::string& texturePath) {
    if (!this->backgroundTexture.loadFromFile(texturePath)) {
        return false;
    }
    this->backgroundSprite = std::make_unique<sf::Sprite>(this->backgroundTexture);

    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        return false;
    }

    nlohmann::json data;
    file >> data;
    file.close();

    for (const auto& layer : data.at("layers")) {
        if (layer.at("type").get<std::string>() == "objectgroup") {
            parse_object_group_layer(layer);
        }
    }
    return true;
}

void Map::parse_object_group_layer(const nlohmann::json& layer) {
    const std::string layer_name = layer.at("name").get<std::string>();

    for (const auto& obj : layer.at("objects")) {
        const float x = obj.at("x").get<float>();
        const float y = obj.at("y").get<float>();

        if (obj.contains("point") && obj.at("point").get<bool>()) {
            parse_point_object(obj, x, y);
        } else if (obj.contains("polygon")) {
            parse_polygon_object(obj, layer_name, x, y);
        } else if (obj.contains("width") && obj.contains("height")) {
            parse_rectangle_object(obj, layer_name, x, y);
        }
    }
}

void Map::parse_point_object(const nlohmann::json& obj, float x, float y) {
    const std::string name = obj.at("name").get<std::string>();
    if (name == "Player1Spawn") {
        this->player1Spawn = {x, y};
    } else if (name == "Player2Spawn") {
        this->player2Spawn = {x, y};
    }
}

void Map::parse_polygon_object(const nlohmann::json& obj, const std::string& layer_name, float x,
                               float y) {
    Wall temp_wall;
    const float rot = obj.contains("rotation") ? obj.at("rotation").get<float>() : 0.F;

    sf::Transform transform;
    transform.translate({x, y});
    if (rot != 0.F) {
        transform.rotate(sf::degrees(rot));
    }

    for (const auto& point : obj.at("polygon")) {
        const sf::Vector2f local_pt(point.at("x").get<float>(), point.at("y").get<float>());
        temp_wall.vertices.push_back(transform.transformPoint(local_pt));
    }

    if (layer_name == "Collision") {
        this->walls.push_back(temp_wall);
    }
}

void Map::parse_rectangle_object(const nlohmann::json& obj, const std::string& layer_name, float x,
                                 float y) {
    Wall temp_wall;
    const float w = obj.at("width").get<float>();
    const float h = obj.at("height").get<float>();
    const float rot = obj.contains("rotation") ? obj.at("rotation").get<float>() : 0.F;

    const std::array<sf::Vector2f, 4> local_vertices = {{{0.F, 0.F}, {w, 0.F}, {w, h}, {0.F, h}}};

    sf::Transform transform;
    transform.translate({x, y});
    if (rot != 0.F) {
        transform.rotate(sf::degrees(rot));
    }

    for (const auto& vert : local_vertices) {
        temp_wall.vertices.push_back(transform.transformPoint(vert));
    }

    if (layer_name == "Collision") {
        this->walls.push_back(temp_wall);
    } else if (layer_name == "Logic") {
        const std::string name = obj.contains("name") ? obj.at("name").get<std::string>() : "";
        if (name == "FinishLine") {
            this->finishLine = temp_wall;
        }
    }
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*this->backgroundSprite, states);
}

const std::vector<Wall>& Map::get_walls() const {
    return this->walls;
}

const Wall& Map::get_finish_line() const {
    return this->finishLine;
}

const sf::Vector2f& Map::get_player1_spawn() const {
    return this->player1Spawn;
}

const sf::Vector2f& Map::get_player2_spawn() const {
    return this->player2Spawn;
}