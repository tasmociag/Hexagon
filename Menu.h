#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include "Board.h"

class Menu{

private:

    sf::Font font; 
    Stage stage;
    Board *board;

    sf::Text *PlayGame = new sf::Text();
    sf::Text *LoadGame = new sf::Text();
    sf::Text *PlayerVsPlayer = new sf::Text();
    sf::Text *PlayerVsBot = new sf::Text();
    sf::Text *ReturnToMenu = new sf::Text();
    sf::Text *Back =  new sf::Text();
    sf::Text *Exit =  new sf::Text();
    sf::Text *PlayerOnePoints =  new sf::Text();
    sf::Text *PlayerTwoPoints =  new sf::Text();
    sf::Text *GameOverText =   new sf::Text();
    sf::Text *SaveGameText =   new sf::Text();
    sf::Text *GameSavedText =   new sf::Text();
    sf::Text *NoSavedGameFile =   new sf::Text();

    sf::RectangleShape *PlayButton = new sf::RectangleShape();
    sf::RectangleShape *LoadButton = new sf::RectangleShape();
    sf::RectangleShape *PlayerVsPlayerButton = new sf::RectangleShape();
    sf::RectangleShape *PlayerVsBotButton = new sf::RectangleShape();
    sf::RectangleShape *ReturnToMenuButton = new sf::RectangleShape();
    sf::RectangleShape *BackButton = new sf::RectangleShape();
    sf::RectangleShape *ExitButton = new sf::RectangleShape();
    sf::RectangleShape *PlayerOnePointsButton = new sf::RectangleShape();
    sf::RectangleShape *PlayerTwoPointsButton = new sf::RectangleShape();
    // sf::RectangleShape *EndOfGameBox= new sf::RectangleShape();
    sf::RectangleShape *SaveGameButton= new sf::RectangleShape();

public:
    Menu();

    void draw(sf::RenderWindow &window);
    void drawMenu(sf::RenderWindow &window)const;
    void drawPlayMenu(sf::RenderWindow &window)const;
    void drawGameOver(sf::RenderWindow &window)const;

    void clickPlayMenu(sf::RenderWindow &window, sf::Vector2i mousePos);
    void clickMenuMenu(sf::RenderWindow &window, sf::Vector2i mousePos);
    void clickReturnToMenu( sf::Vector2i mousePos);
    void clickBack( sf::Vector2i mousePos);
    void clickAction(sf::RenderWindow &window, sf::Vector2i mousePos);
    void clickExit(sf::RenderWindow &window, sf::Vector2i mousePos);
    void clickSaveGame(sf::RenderWindow &window, sf::Vector2i mousePos);

};

#endif

