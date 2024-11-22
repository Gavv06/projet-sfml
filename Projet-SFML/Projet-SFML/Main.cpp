#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

int main() {
    
    RenderWindow window(VideoMode(800, 600), "Morpion");

    Texture cross;
    if (!cross.loadFromFile("assets/cross.jpg")) {
        return -1;
    }

    Texture circle;
    if (!circle.loadFromFile("assets/circle.png")) {
        return -1;
    }

    Font font;
    if (!font.loadFromFile("assets/Dune_Rise.ttf")) {
        return -1;
    }

    RectangleShape line1(Vector2f(500, 5));
    line1.rotate(90);
    line1.setPosition(300, 50);

    RectangleShape line2(Vector2f(500, 5));
    line2.rotate(90);
    line2.setPosition(500, 50);

    RectangleShape line3(Vector2f(600, 5));
    line3.rotate(180);
    line3.setPosition(700, 200);

    RectangleShape line4(Vector2f(600, 5));
    line4.rotate(180);
    line4.setPosition(700, 400);

    RectangleShape case1(Vector2f(160, 160));
    case1.setFillColor(Color::Green);
    case1.setPosition(120, 30);

    RectangleShape case2(Vector2f(160, 160));
    case2.setFillColor(Color::Green);
    case2.setPosition(320, 30);

    RectangleShape case3(Vector2f(160, 160));
    case3.setFillColor(Color::Green);
    case3.setPosition(520, 30);

    RectangleShape case4(Vector2f(160, 160));
    case4.setFillColor(Color::Green);
    case4.setPosition(120, 220);

    RectangleShape case5(Vector2f(160, 160));
    case5.setFillColor(Color::Green);
    case5.setPosition(320, 220);

    RectangleShape case6(Vector2f(160, 160));
    case6.setFillColor(Color::Green);
    case6.setPosition(520, 220);

    RectangleShape case7(Vector2f(160, 160));
    case7.setFillColor(Color::Green);
    case7.setPosition(120, 410);

    RectangleShape case8(Vector2f(160, 160));
    case8.setFillColor(Color::Green);
    case8.setPosition(320, 410);

    RectangleShape case9(Vector2f(160, 160));
    case9.setFillColor(Color::Green);
    case9.setPosition(520, 410);

    Text intro;
    intro.setFont(font);
    intro.setString("r-click for cross / l-click for circle");
    intro.setPosition(70, 600);
    intro.setRotation(270);
    intro.setCharacterSize(18);
    intro.setStyle(Text::Bold | Text::Underlined);


    

    while (window.isOpen()) {
        Event event;
        Vector2i localPosition = Mouse::getPosition(window);
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close(); 
        }

       
        window.clear();
        window.draw(line1);
        window.draw(line2);
        window.draw(line3);
        window.draw(line4);

        window.draw(case1);
        window.draw(case2);
        window.draw(case3);
        window.draw(case4);
        window.draw(case5);
        window.draw(case6);
        window.draw(case7);
        window.draw(case8);
        window.draw(case9);

        window.draw(intro);

        window.display();
    }

    return 0;
}