#include "WinningState.h"
#include "MenuState.h"
#include "Config.h"

WinningState::WinningState(std::unique_ptr<sf::RenderWindow>& window, std::unordered_map<std::string,
    sf::Keyboard::Key> supportedKeys, char winner)
    : State(window, supportedKeys),
    mainMenu(
        Config::instance().getWinningButton("mainMenu").x,
        Config::instance().getWinningButton("mainMenu").y,
        Config::instance().getWinningButton("mainMenu").width,
        Config::instance().getWinningButton("mainMenu").height,
        Config::instance().getWinningButton("mainMenu").text,
        textures["baseButton"], textures["baseButtonClicked"], font),
    playAgain(
        Config::instance().getWinningButton("playAgain").x,
        Config::instance().getWinningButton("playAgain").y,
        Config::instance().getWinningButton("playAgain").width,
        Config::instance().getWinningButton("playAgain").height,
        Config::instance().getWinningButton("playAgain").text,
        textures["baseButton"], textures["baseButtonClicked"], font),
	background(textures[Config::instance().getWinningBackground()]),
    winningText(font, "Player " + std::string(1, winner) + " wins!", Config::instance().getWinningText().fontSize),
    scoresText(font, "", Config::instance().getWinningScoresText().fontSize)
{
    const auto winningTextLayout = Config::instance().getWinningText();
    const auto scoresTextLayout = Config::instance().getWinningScoresText();

    winningText.setFillColor(sf::Color::Black);
	winningText.setPosition(sf::Vector2f(winningTextLayout.x, winningTextLayout.y));
    scoresText.setFillColor(sf::Color::Black);
    scoresText.setPosition(sf::Vector2f(scoresTextLayout.x, scoresTextLayout.y));
    initScores(winner);
}

void WinningState::initScores(char winner)
{
    auto& config = Config::instance();
    int player1score = config.getPlayer1Score();
    int player2score = config.getPlayer2Score();

    if (winner == '1') {
        player1score += 1;
    }
    else if (winner == '2') {
        player2score += 1;
    }

    std::string message;

    if (player1score > player2score) {
        message = "Player 1: " + std::to_string(player1score) + '\n'
            + "Player 2: " + std::to_string(player2score);
    }
    else {
        message = "Player 2: " + std::to_string(player2score) + '\n'
            + "Player 1: " + std::to_string(player1score);
    }

    this->scoresText.setString(message);
    config.setScores(player1score, player2score);
}

void WinningState::updateButtons()
{
    if (this->mainMenu.isPressed())
    {
        this->nextState = std::make_unique<MenuState>(this->window, supportedKeys);
    }
    if (this->playAgain.isPressed())
    {
        this->nextState = std::make_unique<GameState>(this->window, supportedKeys);
    }
}

void WinningState::update(const float& dt)
{
    updateMousePos();
    this->mainMenu.update(this->mousePosView);
    this->playAgain.update(this->mousePosView);
    updateButtons();
}

void WinningState::render(sf::RenderTarget& target)
{
    this->window->draw(this->background, sf::RenderStates::Default);

	this->window->draw(this->winningText, sf::RenderStates::Default);
    this->window->draw(this->scoresText, sf::RenderStates::Default);
    this->mainMenu.render(target);
	this->playAgain.render(target);
}
