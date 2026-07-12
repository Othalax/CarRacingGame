#pragma once

#include "json.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <unordered_map>
#include <vector>

struct ButtonLayout {
    float x;
    float y;
    float width;
    float height;
    std::string text;
};

struct MapEntry {
    std::string name;
    std::string texture;
    std::string configPath;
    float startingAngle;
};

struct SpriteLayout {
    float x;
    float y;
    float scale;
    float rotation;
};

struct TextLayout {
    std::string text;
    int fontSize;
    float x;
    float y;
};

struct CarPhysics {
    float length;
    float maxSteering;
    float maxAcceleration;
    float maxVelocity;
    float brakeDeceleration;
    float freeDeceleration;
    sf::Vector2f spriteOrigin;
    sf::Vector2f spriteScale;
    float accelerationRate;
    float steeringRate;
    float steeringDamping;
    float collisionSpeedMultiplier;
};

class Config {
public:
    static Config& instance();

    const std::string& getWindowTitle() const;
    sf::Vector2u getWindowSize() const;
    unsigned int getFramerate() const;
    bool getVerticalSync() const;

    sf::Vector2f getLogicalSize() const;
    const std::string& getFontPath() const;

    const std::unordered_map<std::string, std::string>& getTexturePaths() const;
    const std::vector<std::string>& getCarTypes() const;
    const std::vector<MapEntry>& getMaps() const;

    int getPlayer1Score() const;
    int getPlayer2Score() const;
    void setScores(int player1, int player2);
    void resetScores();
    void save();

    const std::string& getDefaultPlayer1Car() const;
    const std::string& getDefaultPlayer2Car() const;
    int getDefaultCurrentMap() const;

    const std::unordered_map<std::string, sf::Keyboard::Key>& getSupportedKeys() const;
    std::unordered_map<std::string, sf::Keyboard::Key> getPlayerControls(const std::string& player) const;

    const CarPhysics& getCarPhysics() const;
    int getButtonTextSize() const;

    ButtonLayout getMenuButton(const std::string& name) const;
    ButtonLayout getSettingsButton(const std::string& name) const;
    ButtonLayout getWinningButton(const std::string& name) const;

    const std::string& getMenuBackground() const;
    const std::string& getSettingsBackground() const;
    const std::string& getWinningBackground() const;

    SpriteLayout getSettingsPlayer1View() const;
    SpriteLayout getSettingsPlayer2View() const;
    TextLayout getSettingsMapText() const;
    TextLayout getSettingsMapName() const;
    TextLayout getWinningText() const;
    TextLayout getWinningScoresText() const;

private:
    Config();
    void load();
    void parseSupportedKeys();
    void parseMaps();
    void parseCarTypes();
    void parseTexturePaths();
    void parseCarPhysics();

    static ButtonLayout parseButton(const nlohmann::json& button);
    static SpriteLayout parseSprite(const nlohmann::json& sprite);
    static TextLayout parseText(const nlohmann::json& text, float defaultX = -1.f);

    nlohmann::json data;

    std::unordered_map<std::string, std::string> texturePaths;
    std::vector<std::string> carTypes;
    std::vector<MapEntry> maps;
    std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys;
    CarPhysics carPhysics;
};
