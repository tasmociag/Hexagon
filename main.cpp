#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include "Board.h"
#include "Menu.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(900, 1100), "Hexagon",
    sf::Style::Default,
    sf::ContextSettings(0, 0, 8));
    Menu menu;
    

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed){
                window.close();
            }        
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    menu.clickAction(window,mousePos);
                }
            }

        }
        menu.draw(window);
        window.display();
    }

    return 0;
}


