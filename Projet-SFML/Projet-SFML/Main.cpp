#include <SFML/Graphics.hpp>
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
};

int main() {
    RenderWindow window(VideoMode(800, 600), "Morpion");

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
    intro.setStyle(Text::Bold | Text::Underlined);

    Player currentPlayer = Player::Cross;

    while (window.isOpen()) {
        Event event;
        Vector2i localPosition = Mouse::getPosition(window);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            for (int i = 0; i < GRID_SIZE; ++i) {
                for (int j = 0; j < GRID_SIZE; ++j) {
                    if (grid[i][j].boundingBox.contains(localPosition.x, localPosition.y) && grid[i][j].player == Player::None) {
                        if (currentPlayer == Player::Cross) {
                            grid[i][j].shape.setTexture(&cross);
                            grid[i][j].player = Player::Cross;
                            currentPlayer = Player::Circle;
                        }
                        else if (currentPlayer == Player::Circle) {
                            grid[i][j].shape.setTexture(&circle);
                            grid[i][j].player = Player::Circle;
                            currentPlayer = Player::Cross;
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

        window.display();
    }

    return 0;
}
