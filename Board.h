#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>

enum Stage{
    Start,
    Play,
    PVPGame,
    PVEGame,
    GameOver
};

struct slot{
    sf::CircleShape *shape;
    int ind;
};

struct elementOfQueue{
    int ind;
    int depth;
};

struct SlotValue{
    float value;
    slot* Slot;
    slot* clickedSlot;
};


static sf::Color outlineColor(224,255,255);
static sf::Color fillColor(64,224,208);
static sf::Color clickCenterOutlineColor(255,51,51);
static sf::Color clickFirstOutlineColor(255,172,77);
static sf::Color clickSecondOutlineColor(249,108,0);
static sf::Color PlayerOneOutlineColor(235,109,255);
static sf::Color PlayerOneFillColor(26,255,0);
static sf::Color PlayerTwoFillColor(222,0,255);
static sf::Color PlayerTwoOutlineColor(117,249,102);

class Board{
private:
    
    std::vector<std::vector<int>> board; 
    std::queue<elementOfQueue> kolejka;
    std::vector<SlotValue*>moveValues;
    std::vector<slot*> list;
    slot* clickedSlot;
    Stage gameStage;

    int turn;




public:

    Board();
    
    bool click(sf::RenderWindow &window, sf::Vector2i mousePos);
    bool PlayerMove(sf::RenderWindow &window, sf::Vector2i mousePos);
    void BotMove(sf::RenderWindow &window, sf::Vector2i mousePos);
    void colorEnemy(int player,int ind);
    void BFS();

    bool Turn(sf::RenderWindow &window, sf::Vector2i mousePos);
    bool isEnd(int plyer);
    void draw(sf::RenderWindow &window) const;
    void loadGame();
    Stage getStage()const;
    std::vector<slot*> getList()const;
    void writeTopScores();

    void setSlots();
    void setGameStage(Stage stage);

    int getNumberOfPointsPlayerOne()const;
    int getNumberOfPointsPlayerTwo()const;
    int getWinningPlayer()const;
    int getTurn()const;

};



#endif

