#include "Config.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
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

    parse_texture_paths();
    parse_car_types();
    parse_maps();
    parse_supported_keys();
    parse_car_physics();
}

void Config::parse_texture_paths() {
    texturePaths.clear();
    for (const auto& [name, path] : data.at("textures").items()) {
        texturePaths[name] = path.get<std::string>();
    }
}

void Config::parse_car_types() {
    carTypes = data.at("cars").get<std::vector<std::string>>();
}

void Config::parse_maps() {
    maps.clear();
    for (const auto& map : data.at("maps")) {
        maps.push_back({.name = map.at("name").get<std::string>(),
                        .texture = map.at("texture").get<std::string>(),
                        .configPath = map.at("config").get<std::string>(),
                        .startingAngle = map.at("startingAngle").get<float>()});
    }
}

void Config::parse_supported_keys() {
    supportedKeys.clear();
    for (const auto& [keyName, id] : data.at("supportedKeys").items()) {
        supportedKeys[keyName] = static_cast<sf::Keyboard::Key>(id.get<int>());
    }
}

void Config::parse_car_physics() {
    const auto& car = data.at("car");
    carPhysics.length = car.at("length").get<float>();
    carPhysics.maxSteering = car.at("maxSteering").get<float>();
    carPhysics.maxAcceleration = car.at("maxAcceleration").get<float>();
    carPhysics.maxVelocity = car.at("maxVelocity").get<float>();
    carPhysics.brakeDeceleration = car.at("brakeDeceleration").get<float>();
    carPhysics.freeDeceleration = car.at("freeDeceleration").get<float>();
    carPhysics.spriteOrigin = {car.at("spriteOrigin").at(0).get<float>(),
                               car.at("spriteOrigin").at(1).get<float>()};
    carPhysics.spriteScale = {car.at("spriteScale").at(0).get<float>(),
                              car.at("spriteScale").at(1).get<float>()};
    carPhysics.accelerationRate = car.at("accelerationRate").get<float>();
    carPhysics.steeringRate = car.at("steeringRate").get<float>();
    carPhysics.steeringDamping = car.at("steeringDamping").get<float>();
    carPhysics.collisionSpeedMultiplier = car.at("collisionSpeedMultiplier").get<float>();
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

[[nodiscard]] const std::string& Config::get_window_title() const {
    return data.at("window").at("title").get_ref<const std::string&>();
}

[[nodiscard]] sf::Vector2u Config::get_window_size() const {
    return {data.at("window").at("width").get<unsigned int>(),
            data.at("window").at("height").get<unsigned int>()};
}

[[nodiscard]] unsigned int Config::get_framerate() const {
    return data.at("window").at("framerate").get<unsigned int>();
}

[[nodiscard]] bool Config::get_vertical_sync() const {
    return data.at("window").at("verticalSync").get<bool>();
}

[[nodiscard]] sf::Vector2f Config::get_logical_size() const {
    return {data.at("render").at("logicalWidth").get<float>(),
            data.at("render").at("logicalHeight").get<float>()};
}

[[nodiscard]] const std::string& Config::get_font_path() const {
    return data.at("font").at("path").get_ref<const std::string&>();
}

[[nodiscard]] const std::unordered_map<std::string, std::string>&
Config::get_texture_paths() const {
    return texturePaths;
}

[[nodiscard]] const std::vector<std::string>& Config::get_car_types() const {
    return carTypes;
}

[[nodiscard]] const std::vector<MapEntry>& Config::get_maps() const {
    return maps;
}

[[nodiscard]] int Config::get_player1_score() const {
    return data.at("scores").at("player1").get<int>();
}

[[nodiscard]] int Config::get_player2_score() const {
    return data.at("scores").at("player2").get<int>();
}

void Config::set_scores(int player1, int player2) {
    data.at("scores").at("player1") = player1;
    data.at("scores").at("player2") = player2;
    save();
}

void Config::reset_scores() {
    set_scores(0, 0);
}

[[nodiscard]] const std::string& Config::get_default_player1_car() const {
    return data.at("defaults").at("player1Car").get_ref<const std::string&>();
}

[[nodiscard]] const std::string& Config::get_default_player2_car() const {
    return data.at("defaults").at("player2Car").get_ref<const std::string&>();
}

[[nodiscard]] int Config::get_default_current_map() const {
    return data.at("defaults").at("currentMap").get<int>();
}

[[nodiscard]] const std::unordered_map<std::string, sf::Keyboard::Key>&
Config::get_supported_keys() const {
    return supportedKeys;
}

[[nodiscard]] std::unordered_map<std::string, sf::Keyboard::Key>
Config::get_player_controls(const std::string& player) const {
    std::unordered_map<std::string, sf::Keyboard::Key> controls;
    const auto& player_controls = data.at("controls").at(player);

    for (const auto& [action, keyName] : player_controls.items()) {
        const std::string key = keyName.get<std::string>();
        controls[action] = supportedKeys.at(key);
    }

    return controls;
}

[[nodiscard]] const CarPhysics& Config::get_car_physics() const {
    return carPhysics;
}

[[nodiscard]] int Config::get_button_text_size() const {
    return data.at("buttonTextSize").get<int>();
}

ButtonLayout Config::parse_button(const nlohmann::json& button) {
    return {.x = button.at("x").get<float>(),
            .y = button.at("y").get<float>(),
            .width = button.at("width").get<float>(),
            .height = button.at("height").get<float>(),
            .text = button.at("text").get<std::string>()};
}

SpriteLayout Config::parse_sprite(const nlohmann::json& sprite) {
    return {.x = sprite.at("x").get<float>(),
            .y = sprite.at("y").get<float>(),
            .scale = sprite.at("scale").get<float>(),
            .rotation = sprite.at("rotation").get<float>()};
}

TextLayout Config::parse_text(const nlohmann::json& text, float defaultX) {
    TextLayout layout;
    layout.text = text.contains("text") ? text.at("text").get<std::string>() : "";
    layout.fontSize = text.at("fontSize").get<int>();
    layout.x = text.contains("x") ? text.at("x").get<float>() : defaultX;
    layout.y = text.at("y").get<float>();
    return layout;
}

[[nodiscard]] ButtonLayout Config::get_menu_button(const std::string& name) const {
    return parse_button(data.at("menu").at("buttons").at(name));
}

[[nodiscard]] ButtonLayout Config::get_settings_button(const std::string& name) const {
    return parse_button(data.at("settings").at("buttons").at(name));
}

[[nodiscard]] ButtonLayout Config::get_winning_button(const std::string& name) const {
    return parse_button(data.at("winning").at("buttons").at(name));
}

[[nodiscard]] const std::string& Config::get_menu_background() const {
    return data.at("menu").at("background").get_ref<const std::string&>();
}

[[nodiscard]] const std::string& Config::get_settings_background() const {
    return data.at("settings").at("background").get_ref<const std::string&>();
}

[[nodiscard]] const std::string& Config::get_winning_background() const {
    return data.at("winning").at("background").get_ref<const std::string&>();
}

[[nodiscard]] SpriteLayout Config::get_settings_player1_view() const {
    return parse_sprite(data.at("settings").at("player1View"));
}

[[nodiscard]] SpriteLayout Config::get_settings_player2_view() const {
    return parse_sprite(data.at("settings").at("player2View"));
}

[[nodiscard]] TextLayout Config::get_settings_map_text() const {
    return parse_text(data.at("settings").at("mapText"));
}

[[nodiscard]] TextLayout Config::get_settings_map_name() const {
    return parse_text(data.at("settings").at("mapName"));
}

[[nodiscard]] TextLayout Config::get_winning_text() const {
    return parse_text(data.at("winning").at("winningText"));
}

[[nodiscard]] TextLayout Config::get_winning_scores_text() const {
    return parse_text(data.at("winning").at("scoresText"));
}
