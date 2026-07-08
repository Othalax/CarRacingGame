#include "Map.h"
#include <fstream>
#include <iostream>
#include "json.hpp"

Map::Map() {
    this->player1Spawn = { 100.f, 100.f };
    this->player2Spawn = { 150.f, 100.f };
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

    for (const auto& layer : data["layers"]) {
        std::string layerName = layer["name"].get<std::string>();
        std::string layerType = layer["type"].get<std::string>();

        if (layerType == "objectgroup") {
            for (const auto& obj : layer["objects"]) {
                float x = obj["x"].get<float>();
                float y = obj["y"].get<float>();
                
                if (obj.contains("point") && obj["point"].get<bool>()) {
                    std::string name = obj["name"].get<std::string>();
                    if (name == "Player1Spawn") this->player1Spawn = { x, y };
                    else if (name == "Player2Spawn") this->player2Spawn = { x, y };
                    continue;
                }

                if (obj.contains("polygon")) {
                    Wall tempWall;
                    float rot = obj.contains("rotation") ? obj["rotation"].get<float>() : 0.f;

                    sf::Transform transform;
                    transform.translate({ x, y });
                    if (rot != 0.f) {
                        transform.rotate(sf::degrees(rot));
                    }

                    for (const auto& point : obj["polygon"]) {
                        sf::Vector2f localPt(point["x"].get<float>(), point["y"].get<float>());
                        tempWall.vertices.push_back(transform.transformPoint(localPt));
                    }

                    if (layerName == "Collision") {
                        this->walls.push_back(tempWall);
                    }
                }
                else if (obj.contains("width") && obj.contains("height")) {
                    Wall tempWall;
                    float w = obj["width"].get<float>();
                    float h = obj["height"].get<float>();
                    float rot = obj.contains("rotation") ? obj["rotation"].get<float>() : 0.f;

                    sf::Vector2f localVertices[4] = {
                        { 0.f, 0.f }, 
                        { w, 0.f },   
                        { w, h },    
                        { 0.f, h }   
                    };

                    sf::Transform transform;
                    transform.translate({ x, y });
                    if (rot != 0.f) {
                        transform.rotate(sf::degrees(rot));
                    }

                    for (int i = 0; i < 4; ++i) {
                        tempWall.vertices.push_back(transform.transformPoint(localVertices[i]));
                    }

                    if (layerName == "Collision") {
                        this->walls.push_back(tempWall);
                    }

                    else if (layerName == "Logic") {
                        std::string name = obj.contains("name") ? obj["name"].get<std::string>() : "";

                        if (name == "FinishLine") {
                            this->finishLine = tempWall;
                        }
                    }
                }
            }
        }
    }
    return true;
}

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(*this->backgroundSprite, states);
}

const std::vector<Wall>& Map::getWalls() const { 
    return this->walls; 
}

const Wall& Map::getFinishLine() const { 
    return this->finishLine; 
}

const sf::Vector2f& Map::getPlayer1Spawn() const { 
    return this->player1Spawn; 
}

const sf::Vector2f& Map::getPlayer2Spawn() const { 
    return this->player2Spawn; 
}