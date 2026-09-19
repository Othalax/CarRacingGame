#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <nlohmann/json.hpp>
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

    [[nodiscard]] const std::string& get_window_title() const;
    [[nodiscard]] sf::Vector2u get_window_size() const;
    [[nodiscard]] unsigned int get_framerate() const;
    [[nodiscard]] bool get_vertical_sync() const;

    [[nodiscard]] sf::Vector2f get_logical_size() const;
    [[nodiscard]] const std::string& get_font_path() const;

    [[nodiscard]] const std::unordered_map<std::string, std::string>& get_texture_paths() const;
    [[nodiscard]] const std::vector<std::string>& get_car_types() const;
    [[nodiscard]] const std::vector<MapEntry>& get_maps() const;

    [[nodiscard]] int get_player1_score() const;
    [[nodiscard]] int get_player2_score() const;
    void set_scores(int player1, int player2);
    void reset_scores();
    void save();

    [[nodiscard]] const std::string& get_default_player1_car() const;
    [[nodiscard]] const std::string& get_default_player2_car() const;
    [[nodiscard]] int get_default_current_map() const;

    [[nodiscard]] const std::unordered_map<std::string, sf::Keyboard::Key>&
    get_supported_keys() const;
    [[nodiscard]] std::unordered_map<std::string, sf::Keyboard::Key>
    get_player_controls(const std::string& player) const;

    [[nodiscard]] const CarPhysics& get_car_physics() const;
    [[nodiscard]] int get_button_text_size() const;

    [[nodiscard]] ButtonLayout get_menu_button(const std::string& name) const;
    [[nodiscard]] ButtonLayout get_settings_button(const std::string& name) const;
    [[nodiscard]] ButtonLayout get_winning_button(const std::string& name) const;

    [[nodiscard]] const std::string& get_menu_background() const;
    [[nodiscard]] const std::string& get_settings_background() const;
    [[nodiscard]] const std::string& get_winning_background() const;

    [[nodiscard]] SpriteLayout get_settings_player1_view() const;
    [[nodiscard]] SpriteLayout get_settings_player2_view() const;
    [[nodiscard]] TextLayout get_settings_map_text() const;
    [[nodiscard]] TextLayout get_settings_map_name() const;
    [[nodiscard]] TextLayout get_winning_text() const;
    [[nodiscard]] TextLayout get_winning_scores_text() const;

  private:
    Config();
    void load();
    void parse_supported_keys();
    void parse_maps();
    void parse_car_types();
    void parse_texture_paths();
    void parse_car_physics();

    static ButtonLayout parse_button(const nlohmann::json& button);
    static SpriteLayout parse_sprite(const nlohmann::json& sprite);
    static TextLayout parse_text(const nlohmann::json& text, float defaultX = -1.F);

    nlohmann::json data;

    std::unordered_map<std::string, std::string> texturePaths;
    std::vector<std::string> carTypes;
    std::vector<MapEntry> maps;
    std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys;
    CarPhysics carPhysics;
};

#endif // CONFIG_H