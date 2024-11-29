#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <array>
#include <cstdlib>
#include <chrono>

using namespace sf;

const int GRID_SIZE = 3;
const float CASE_SIZE = 160.f;

enum class Player { None, Cross, Circle };

struct Case {
    RectangleShape shape;
    Player player = Player::None;
    bool isCracked = false;

    Case() {
        player = Player::None;
        shape.setSize(Vector2f(CASE_SIZE, CASE_SIZE));
        shape.setFillColor(Color::Green);
    }

    Case(float x, float y) {
        shape.setSize(Vector2f(CASE_SIZE, CASE_SIZE));
        shape.setPosition(x, y);
        shape.setFillColor(Color::Green);
    }

    void reset() {
        player = Player::None;
        isCracked = false;
        shape.setTexture(nullptr);
        shape.setFillColor(Color::Green);
    }
};

bool checkWinner(const std::array<std::array<Case, GRID_SIZE>, GRID_SIZE>& grid, Player player) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        if (grid[i][0].player == player && grid[i][1].player == player && grid[i][2].player == player)
            return true;
        if (grid[0][i].player == player && grid[1][i].player == player && grid[2][i].player == player)
            return true;
    }
    if (grid[0][0].player == player && grid[1][1].player == player && grid[2][2].player == player)
        return true;
    if (grid[0][2].player == player && grid[1][1].player == player && grid[2][0].player == player)
        return true;

    return false;
}

bool isDraw(const std::array<std::array<Case, GRID_SIZE>, GRID_SIZE>& grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j].player == Player::None && !grid[i][j].isCracked)
                return false;
        }
    }
    return true;
}

int main() {
    std::srand(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));

    RenderWindow window(VideoMode(800, 600), "Morpion 0.5 final mix");

    Texture crossTexture, circleTexture, crackTexture;
    if (!crossTexture.loadFromFile("assets/cross.jpg") || !circleTexture.loadFromFile("assets/circle.png") || !crackTexture.loadFromFile("assets/fissure.png")) {
        std::cerr << "Erreur de chargement des textures !" << std::endl;
        return -1;
    }

    Font font;
    if (!font.loadFromFile("assets/Dune_Rise.ttf")) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
        return -1;
    }

    SoundBuffer clickSoundBuffer;
    if (!clickSoundBuffer.loadFromFile("assets/sound_click.wav")) {
        std::cerr << "Erreur de chargement du son !" << std::endl;
        return -1;
    }

    Sound clickSound;
    clickSound.setBuffer(clickSoundBuffer);
    clickSound.setVolume(80.f);

    RectangleShape gridLines[] = {
        RectangleShape(Vector2f(500, 5)), RectangleShape(Vector2f(500, 5)),
        RectangleShape(Vector2f(600, 5)), RectangleShape(Vector2f(600, 5))
    };

    gridLines[0].rotate(90); gridLines[0].setPosition(300, 50);
    gridLines[1].rotate(90); gridLines[1].setPosition(500, 50);
    gridLines[2].setPosition(100, 200);
    gridLines[3].setPosition(100, 400);

    std::array<std::array<Case, GRID_SIZE>, GRID_SIZE> grid = { {
        { Case(120, 30), Case(320, 30), Case(520, 30) },
        { Case(120, 220), Case(320, 220), Case(520, 220) },
        { Case(120, 410), Case(320, 410), Case(520, 410) }
    } };

    Text introText;
    introText.setFont(font);
    introText.setString("Cross first, left-click to place.");
    introText.setPosition(70, 550);
    introText.setRotation(270);
    introText.setCharacterSize(18);
    introText.setFillColor(Color::White);
    introText.setStyle(Text::Bold | Text::Underlined);

    Text endText;
    endText.setFont(font);
    endText.setCharacterSize(30);
    endText.setStyle(Text::Bold | Text::Underlined);
    endText.setPosition(300, 250);
    endText.setFillColor(Color::White);

    RectangleShape restartButton(Vector2f(300, 50));
    restartButton.setPosition(750, 300);
    restartButton.rotate(90);
    restartButton.setFillColor(Color::Blue);

    Text restartText;
    restartText.setFont(font);
    restartText.setString("Restart");
    restartText.setCharacterSize(24);
    restartText.setStyle(Text::Bold);
    restartText.setPosition(750, 315);
    restartText.rotate(90);
    restartText.setFillColor(Color::White);

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setPosition(40, 550);
    scoreText.setFillColor(Color::White);
    scoreText.setRotation(270);

    Player currentPlayer = Player::Cross;
    bool gameEnded = false;
    int crossScore = 0, circleScore = 0, drawScore = 0;

    RectangleShape crackButton(Vector2f(100, 50));
    crackButton.setPosition(750, 200);
    crackButton.rotate(90);
    crackButton.setFillColor(Color::Red);

    Text crackText;
    crackText.setFont(font);
    crackText.setString("Crack");
    crackText.setCharacterSize(20);
    crackText.setFillColor(Color::White);
    crackText.setPosition(750, 200);
    crackText.rotate(90);

    bool crackEnabled = true;

    while (window.isOpen()) {
        Event event;
        Vector2i localPosition = Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            clickSound.play();

            if (crackButton.getGlobalBounds().contains(localPosition.x, localPosition.y) && crackEnabled) {
                crackButton.setFillColor(Color::Green);
                crackEnabled = false;

                int row = rand() % GRID_SIZE;
                int col = rand() % GRID_SIZE;
                while (grid[row][col].player != Player::None || grid[row][col].isCracked) {
                    row = rand() % GRID_SIZE;
                    col = rand() % GRID_SIZE;
                }
                grid[row][col].shape.setTexture(&crackTexture);
                grid[row][col].isCracked = true;
            }

            if (gameEnded && restartButton.getGlobalBounds().contains(localPosition.x, localPosition.y)) {
                for (int i = 0; i < GRID_SIZE; ++i) {
                    for (int j = 0; j < GRID_SIZE; ++j) {
                        grid[i][j].reset();
                    }
                }
                currentPlayer = Player::Cross;
                gameEnded = false;
                endText.setString("");
                crackButton.setFillColor(Color::Red);
                crackEnabled = true;
            }
            else if (!gameEnded) {
                for (int i = 0; i < GRID_SIZE; ++i) {
                    for (int j = 0; j < GRID_SIZE; ++j) {
                        if (grid[i][j].shape.getGlobalBounds().contains(localPosition.x, localPosition.y) &&
                            grid[i][j].player == Player::None && !grid[i][j].isCracked) {
                            if (currentPlayer == Player::Cross) {
                                grid[i][j].shape.setTexture(&crossTexture);
                                grid[i][j].shape.setFillColor(Color::Red);
                                grid[i][j].player = Player::Cross;
                                currentPlayer = Player::Circle;
                            }
                            else {
                                grid[i][j].shape.setTexture(&circleTexture);
                                grid[i][j].shape.setFillColor(Color::Blue);
                                grid[i][j].player = Player::Circle;
                                currentPlayer = Player::Cross;
                            }

                            if (checkWinner(grid, Player::Cross)) {
                                endText.setString("Cross Wins!");
                                endText.setFillColor(Color::Blue);
                                crossScore++;
                                gameEnded = true;
                            }
                            else if (checkWinner(grid, Player::Circle)) {
                                endText.setString("Circle Wins!");
                                endText.setFillColor(Color::Blue);
                                circleScore++;
                                gameEnded = true;
                            }
                            else if (isDraw(grid)) {
                                endText.setString("It's a Draw!");
                                endText.setFillColor(Color::Blue);
                                drawScore++;
                                gameEnded = true;
                            }
                        }
                    }
                }
            }
        }

        scoreText.setString("Cross: " + std::to_string(crossScore) + " Circle: " + std::to_string(circleScore) + " Draws: " + std::to_string(drawScore));

        window.clear();

        window.draw(gridLines[0]);
        window.draw(gridLines[1]);
        window.draw(gridLines[2]);
        window.draw(gridLines[3]);

        window.draw(introText);

        window.draw(restartButton);
        window.draw(restartText);

        window.draw(scoreText);

        window.draw(crackButton);
        window.draw(crackText);

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                window.draw(grid[i][j].shape);
            }
        }

        if (gameEnded) {
            window.draw(endText);
        }

        window.display();
    }

    return 0;
}
