#include "Config.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

Config& Config::instance() {
    static Config config;
    return config;
}

Config::Config() {
    load();
}

void Config::load() {
    std::ifstream file("config/config.json");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open config/config.json");
    }

    file >> data;
    file.close();

    parseTexturePaths();
    parseCarTypes();
    parseMaps();
    parseSupportedKeys();
    parseCarPhysics();
}

void Config::parseTexturePaths() {
    texturePaths.clear();
    for (auto& [name, path] : data["textures"].items()) {
        texturePaths[name] = path.get<std::string>();
    }
}

void Config::parseCarTypes() {
    carTypes = data["cars"].get<std::vector<std::string>>();
}

void Config::parseMaps() {
    maps.clear();
    for (const auto& map : data["maps"]) {
        maps.push_back({
            map["name"].get<std::string>(),
            map["texture"].get<std::string>(),
            map["config"].get<std::string>(),
            map["startingAngle"].get<float>()
        });
    }
}

void Config::parseSupportedKeys() {
    supportedKeys.clear();
    for (auto& [keyName, id] : data["supportedKeys"].items()) {
        supportedKeys[keyName] = static_cast<sf::Keyboard::Key>(id.get<int>());
    }
}

void Config::parseCarPhysics() {
    const auto& car = data["car"];
    carPhysics.length = car["length"].get<float>();
    carPhysics.maxSteering = car["maxSteering"].get<float>();
    carPhysics.maxAcceleration = car["maxAcceleration"].get<float>();
    carPhysics.maxVelocity = car["maxVelocity"].get<float>();
    carPhysics.brakeDeceleration = car["brakeDeceleration"].get<float>();
    carPhysics.freeDeceleration = car["freeDeceleration"].get<float>();
    carPhysics.spriteOrigin = {
        car["spriteOrigin"][0].get<float>(),
        car["spriteOrigin"][1].get<float>()
    };
    carPhysics.spriteScale = {
        car["spriteScale"][0].get<float>(),
        car["spriteScale"][1].get<float>()
    };
    carPhysics.accelerationRate = car["accelerationRate"].get<float>();
    carPhysics.steeringRate = car["steeringRate"].get<float>();
    carPhysics.steeringDamping = car["steeringDamping"].get<float>();
    carPhysics.collisionSpeedMultiplier = car["collisionSpeedMultiplier"].get<float>();
}

void Config::save() {
    std::ofstream file("config/config.json");
    if (!file.is_open()) {
        std::cout << "Error saving config/config.json\n";
        return;
    }

    file << data.dump(2);
    file.close();
}

const std::string& Config::getWindowTitle() const {
    return data["window"]["title"].get_ref<const std::string&>();
}

sf::Vector2u Config::getWindowSize() const {
    return {
        data["window"]["width"].get<unsigned int>(),
        data["window"]["height"].get<unsigned int>()
    };
}

unsigned int Config::getFramerate() const {
    return data["window"]["framerate"].get<unsigned int>();
}

bool Config::getVerticalSync() const {
    return data["window"]["verticalSync"].get<bool>();
}

sf::Vector2f Config::getLogicalSize() const {
    return {
        data["render"]["logicalWidth"].get<float>(),
        data["render"]["logicalHeight"].get<float>()
    };
}

const std::string& Config::getFontPath() const {
    return data["font"]["path"].get_ref<const std::string&>();
}

const std::unordered_map<std::string, std::string>& Config::getTexturePaths() const {
    return texturePaths;
}

const std::vector<std::string>& Config::getCarTypes() const {
    return carTypes;
}

const std::vector<MapEntry>& Config::getMaps() const {
    return maps;
}

int Config::getPlayer1Score() const {
    return data["scores"]["player1"].get<int>();
}

int Config::getPlayer2Score() const {
    return data["scores"]["player2"].get<int>();
}

void Config::setScores(int player1, int player2) {
    data["scores"]["player1"] = player1;
    data["scores"]["player2"] = player2;
    save();
}

void Config::resetScores() {
    setScores(0, 0);
}

const std::string& Config::getDefaultPlayer1Car() const {
    return data["defaults"]["player1Car"].get_ref<const std::string&>();
}

const std::string& Config::getDefaultPlayer2Car() const {
    return data["defaults"]["player2Car"].get_ref<const std::string&>();
}

int Config::getDefaultCurrentMap() const {
    return data["defaults"]["currentMap"].get<int>();
}

const std::unordered_map<std::string, sf::Keyboard::Key>& Config::getSupportedKeys() const {
    return supportedKeys;
}

std::unordered_map<std::string, sf::Keyboard::Key> Config::getPlayerControls(const std::string& player) const {
    std::unordered_map<std::string, sf::Keyboard::Key> controls;
    const auto& playerControls = data["controls"][player];

    for (auto& [action, keyName] : playerControls.items()) {
        const std::string key = keyName.get<std::string>();
        controls[action] = supportedKeys.at(key);
    }

    return controls;
}

const CarPhysics& Config::getCarPhysics() const {
    return carPhysics;
}

int Config::getButtonTextSize() const {
    return data["buttonTextSize"].get<int>();
}

ButtonLayout Config::parseButton(const nlohmann::json& button) {
    return {
        button["x"].get<float>(),
        button["y"].get<float>(),
        button["width"].get<float>(),
        button["height"].get<float>(),
        button["text"].get<std::string>()
    };
}

SpriteLayout Config::parseSprite(const nlohmann::json& sprite) {
    return {
        sprite["x"].get<float>(),
        sprite["y"].get<float>(),
        sprite["scale"].get<float>(),
        sprite["rotation"].get<float>()
    };
}

TextLayout Config::parseText(const nlohmann::json& text, float defaultX) {
    TextLayout layout;
    layout.text = text.contains("text") ? text["text"].get<std::string>() : "";
    layout.fontSize = text["fontSize"].get<int>();
    layout.x = text.contains("x") ? text["x"].get<float>() : defaultX;
    layout.y = text["y"].get<float>();
    return layout;
}

ButtonLayout Config::getMenuButton(const std::string& name) const {
    return parseButton(data["menu"]["buttons"][name]);
}

ButtonLayout Config::getSettingsButton(const std::string& name) const {
    return parseButton(data["settings"]["buttons"][name]);
}

ButtonLayout Config::getWinningButton(const std::string& name) const {
    return parseButton(data["winning"]["buttons"][name]);
}

const std::string& Config::getMenuBackground() const {
    return data["menu"]["background"].get_ref<const std::string&>();
}

const std::string& Config::getSettingsBackground() const {
    return data["settings"]["background"].get_ref<const std::string&>();
}

const std::string& Config::getWinningBackground() const {
    return data["winning"]["background"].get_ref<const std::string&>();
}

SpriteLayout Config::getSettingsPlayer1View() const {
    return parseSprite(data["settings"]["player1View"]);
}

SpriteLayout Config::getSettingsPlayer2View() const {
    return parseSprite(data["settings"]["player2View"]);
}

TextLayout Config::getSettingsMapText() const {
    return parseText(data["settings"]["mapText"]);
}

TextLayout Config::getSettingsMapName() const {
    return parseText(data["settings"]["mapName"]);
}

TextLayout Config::getWinningText() const {
    return parseText(data["winning"]["winningText"]);
}

TextLayout Config::getWinningScoresText() const {
    return parseText(data["winning"]["scoresText"]);
}
