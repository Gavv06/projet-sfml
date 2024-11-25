#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <array>

using namespace sf;

const int GRID_SIZE = 3;
const float CASE_SIZE = 160.f;

enum class Player { None, Cross, Circle };

struct Case {
    RectangleShape shape;
    Player player = Player::None;
    FloatRect boundingBox;

    Case(float x, float y) {
        shape.setSize(Vector2f(CASE_SIZE, CASE_SIZE));
        shape.setPosition(x, y);
        shape.setFillColor(Color::Green);
        boundingBox = shape.getGlobalBounds();
    }

    void reset() {
        player = Player::None;
        shape.setTexture(nullptr);  // Reset the texture
    }
};

bool checkWinner(const std::array<std::array<Case, GRID_SIZE>, GRID_SIZE>& grid, Player player) {
    
    for (int i = 0; i < GRID_SIZE; ++i) {
        if (grid[i][0].player == player && grid[i][1].player == player && grid[i][2].player == player)
            return true;
    }

    
    for (int i = 0; i < GRID_SIZE; ++i) {
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
            if (grid[i][j].player == Player::None)
                return false;
        }
    }
    return true;
}

int main() {
    RenderWindow window(VideoMode(800, 600), "Morpion 0.5 final mix");

    Texture cross, circle;
    if (!cross.loadFromFile("assets/cross.jpg") || !circle.loadFromFile("assets/circle.png")) {
        std::cerr << "Erreur de chargement des textures !" << std::endl;
        return -1;
    }

    Font font;
    if (!font.loadFromFile("assets/Dune_Rise.ttf")) {
        std::cerr << "Erreur de chargement de la police !" << std::endl;
        return -1;
    }

    SoundBuffer click;
    if (!click.loadFromFile("assets/sound_click.wav")) {
        std::cout << "Erreur : Impossible de charger le fichier audio." << std::endl;
        return -1;
    }

    RectangleShape line1(Vector2f(500, 5));
    line1.rotate(90);
    line1.setPosition(300, 50);

    RectangleShape line2(Vector2f(500, 5));
    line2.rotate(90);
    line2.setPosition(500, 50);

    RectangleShape line3(Vector2f(600, 5));
    line3.setPosition(100, 200);

    RectangleShape line4(Vector2f(600, 5));
    line4.setPosition(100, 400);

    std::array<std::array<Case, GRID_SIZE>, GRID_SIZE> grid = { {
        {{ Case(120, 30), Case(320, 30), Case(520, 30) }},
        {{ Case(120, 220), Case(320, 220), Case(520, 220) }},
        {{ Case(120, 410), Case(320, 410), Case(520, 410) }},
    } };

    Text intro;
    intro.setFont(font);
    intro.setString("cross first, l-click to change");
    intro.setPosition(70, 550);
    intro.setRotation(270);
    intro.setCharacterSize(18);
    intro.setFillColor(Color::Blue);
    intro.setStyle(Text::Bold | Text::Underlined);

    Text endText;
    endText.setFont(font);
    endText.setCharacterSize(30);
    endText.setStyle(Text::Bold | Text::Underlined);
    endText.setPosition(300, 250);

    
    RectangleShape restartButton(Vector2f(300, 50));
    restartButton.setPosition(750, 300);
    restartButton.rotate(90);
    restartButton.setFillColor(Color::Blue);

    Text restartText;
    restartText.setFont(font);
    restartText.setString("Recommencer");
    restartText.setCharacterSize(24);
    restartText.setStyle(Text::Bold);
    restartText.setPosition(750, 315);
    restartText.rotate(90);
    restartText.setFillColor(Color::White);

    Player currentPlayer = Player::Cross;
    bool gameEnded = false;

    while (window.isOpen()) {
        Event event;
        Vector2i localPosition = Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (gameEnded && restartButton.getGlobalBounds().contains(localPosition.x, localPosition.y)) {
                for (int i = 0; i < GRID_SIZE; ++i) {
                    for (int j = 0; j < GRID_SIZE; ++j) {
                        grid[i][j].reset();
                    }
                }
                currentPlayer = Player::Cross;
                gameEnded = false;
                endText.setString("");
                
            }
            else if (!gameEnded) {
                for (int i = 0; i < GRID_SIZE; ++i) {
                    for (int j = 0; j < GRID_SIZE; ++j) {
                        if (grid[i][j].boundingBox.contains(localPosition.x, localPosition.y) && grid[i][j].player == Player::None) {
                            if (currentPlayer == Player::Cross) {
                                Sound clickSound;
                                clickSound.play();
                                grid[i][j].shape.setTexture(&cross);
                                grid[i][j].player = Player::Cross;
                                currentPlayer = Player::Circle;
                            }
                            else if (currentPlayer == Player::Circle) {
                                Sound clickSound;
                                clickSound.play();
                                grid[i][j].shape.setTexture(&circle);
                                grid[i][j].player = Player::Circle;
                                currentPlayer = Player::Cross;
                            }

                            if (checkWinner(grid, Player::Cross)) {
                                endText.setString("Cross Wins!");
                                endText.setFillColor(Color::Blue);
                                gameEnded = true;
                            }
                            else if (checkWinner(grid, Player::Circle)) {
                                endText.setString("Circle Wins!");
                                endText.setFillColor(Color::Blue);
                                gameEnded = true;
                            }
                            else if (isDraw(grid)) {
                                endText.setString("It's a Draw!");
                                endText.setFillColor(Color::Blue);
                                gameEnded = true;
                            }
                        }
                    }
                }
            }
        }

        window.clear();

        window.draw(line1);
        window.draw(line2);
        window.draw(line3);
        window.draw(line4);

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                window.draw(grid[i][j].shape);
            }
        }

        window.draw(intro);
        window.draw(restartButton);
        window.draw(restartText);

        if (gameEnded) {
            window.draw(endText);
        }

        window.display();
    }

    return 0;
}
