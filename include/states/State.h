#ifndef STATE_H
#define STATE_H

#include "Config.h"

class State {
  public:
    State(sf::RenderWindow* window,
          std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys);
    virtual ~State() = default;
    State(const State&) = delete;
    State& operator=(const State&) = delete;
    State(State&&) noexcept = default;
    State& operator=(State&&) = delete;

    const bool& get_quit() const;
    std::unique_ptr<State> get_next_state();
    void end_state();
    void update_mouse_pos();
    virtual void update(const float& dt);
    virtual void render(sf::RenderTarget& target);

  protected:
    sf::RenderWindow& get_window() const;
    std::unordered_map<std::string, sf::Texture>& get_textures();
    const std::unordered_map<std::string, sf::Texture>& get_textures() const;
    sf::Font& get_font();
    const sf::Font& get_font() const;
    const std::unordered_map<std::string, sf::Keyboard::Key>& get_supported_keys() const;

    const sf::Vector2i& get_mouse_pos_screen() const;
    const sf::Vector2i& get_mouse_pos_window() const;
    const sf::Vector2f& get_mouse_pos_view() const;

    void set_next_state(std::unique_ptr<State> state);

    static std::string player1car;
    static std::string player2car;
    static std::vector<MapEntry> maps;
    static int current_map;

    void init_textures();
    void init_font();

  private:
    sf::RenderWindow* window;
    std::unordered_map<std::string, sf::Texture> textures;
    sf::Font font;
    std::unordered_map<std::string, sf::Keyboard::Key> supportedKeys;

    sf::Vector2i mousePosScreen;
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView;

    bool quit{false};
    std::unique_ptr<State> nextState;
};

#endif // STATE_H