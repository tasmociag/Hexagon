#include "Board.h"
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <queue>
#include <algorithm>

//                 1
//             2       3
//         4       5       6
//     7       8       9       1O  
// 11     12      13      14      15
//    16      17      18      19  
// 20     21              23      24
//    25      26      27      28  
// 29     30      31      32      33
//    34                      37  
// 38     39      40      41      42
//    43      44      45      46  
// 47     48      49      50      51
//    52      53      54      55  
//        56      57      58
//            59      60
//                61
//



Board::Board(){

    list.clear();
    board.clear();
    setSlots();
    clickedSlot = nullptr;
    turn = 0;


    board = {
        {},
        {2,3,5},
        {1,4,5,8},
        {1,5,6,9},
        {2,7,8,12},
        {2,3,8,9,1,13},
        {3,9,10,14},
        {4,11,12,16},
        {4,5,12,13,2,17},
        {5,6,13,14,3,18},
        {6,14,15,19},
        {7,16,20},
        {7,8,16,17,4,21},
        {8,5,9,17,18},
        {6,9,10,18,23,19},
        {10,19,24},
        {7,11,12,21,20,25},
        {8,13,12,21,26},
        {13,9,14,23,27},
        {10,14,15,23,24,28},
        {11,16,25,29},
        {12,16,17,25,30,26},
        {},
        {18,14,19,32,27,28},
        {15,19,28,33},
        {20,16,21,30,34,29},
        {17,21,30,31},
        {18,23,32,31},
        {19,23,24,32,33,37},
        {20,25,34,38},
        {21,25,26,34,39},
        {26,27,40},
        {27,23,28,37,41},
        {24,28,37,42},
        {25,29,30,39,38,43},
        {},
        {},
        {32,28,33,42,41,46},
        {29,34,43,47},
        {30,34,43,44,48},
        {31,44,49,45},
        {32,37,46,50,45},
        {33,37,46,51},
        {34,38,39,48,52,47},
        {39,40,49,48,53},
        {40,41,50,54,49},
        {41,37,42,51,55,50},
        {38,43,52},
        {39,44,43,53,56,52},
        {40,44,45,54,53,57},
        {41,45,46,55,58,54},
        {42,46,55},
        {47,43,48,56},
        {44,48,49,57,59,56},
        {45,49,50,57,58,60},
        {46,50,51,58},
        {52,48,53,59},
        {49,53,54,60,59,61},
        {50,55,54,60},
        {56,53,57,61},
        {57,54,58,61},
        {59,57,60}
    };
}

// static sf::Color fillColor(64,224,208);
// static sf::Color outlineColor(224,255,255);
// static sf::Color clickCenterOutlineColor(255,51,51);
// static sf::Color clickFirstOutlineColor(255,172,77);
// static sf::Color clickSecondOutlineColor(249,108,0);
// static sf::Color PlayerOneFillColor(222,0,255);
// static sf::Color PlayerOneOutlineColor(235,109,255);
// static sf::Color PlayerTwoFillColor(26,255,0);
// static sf::Color PlayerTwoOutlineColor(117,249,102);

bool Board::Turn(sf::RenderWindow &window, sf::Vector2i mousePos){
    ///wykonuje albo jeden ruch gracza albo ruch gracza oraz automatycznie ruch bota w zaleznosci od trybu gry
    
    if(gameStage == PVEGame){
        if(PlayerMove(window,mousePos)){
            if(!isEnd((turn+1)%2)){
                draw(window);
                window.display();
                sleep(1);
                BotMove(window, mousePos);
                // std::cout << "PlayerOneNumberOfPoints: " << PlayerOneNumberOfPoints << "\nPlayerTwoNumberOfPoints: " << PlayerTwoNumberOfPoints << "\n";
                return isEnd((turn+1)%2);
            }else{
                return 1;
            }
        } else {
            return 0;
        }
    }else{
        if(PlayerMove(window,mousePos)){
            return isEnd((turn+1)%2);
        } else {
            return 0;
        }
    }

    return 0;
}


void Board::BotMove(sf::RenderWindow &window, sf::Vector2i mousePos){
    ///wykonuje ruch bota. Ruch jest wybierany na zasadzie atrakcyjnosci kazdego ruchy. Kazdy mozliwu ruch bota jest oceniany pod wzgledem atrakcyjnosci w zaleznosci od ilosci zbitych pionkow, tego czy sie rusza o jeden czy o dwa pola oraz tego ile moze stracic punktow jezeli ruszy sie o 2 pola.
    std::cout << "botMove\n";
    moveValues.clear();

    float valueOfSlot = 0;

    for(slot *sl:list){
        if(sl->shape->getFillColor()==PlayerTwoFillColor){

            clickedSlot = sl;

            for(slot* sl2:list)
                sl2->shape->setOutlineColor(outlineColor);

            while(!kolejka.empty())
                kolejka.pop();
            elementOfQueue element = {sl->ind , 0};
            kolejka.push(element);
            BFS();

            for(slot* sl2:list){

                valueOfSlot = 0;

                SlotValue *sv = new SlotValue;
                sv->Slot = sl2;
                sv->clickedSlot = nullptr;
                sv->value = 0;


                if(sl2->shape->getOutlineColor()==clickFirstOutlineColor){
                    valueOfSlot+=2;
                }
                if(sl2->shape->getOutlineColor()==clickSecondOutlineColor){
                    valueOfSlot+=1;
                    for(int i=0;i<board[sl->ind].size();i++){
                        if(list[board[sl->ind][1]-1]->shape->getFillColor() == PlayerOneFillColor){
                            valueOfSlot-=0.6;
                        }
                    }
                    sv->clickedSlot=clickedSlot;
                }

                if(sl2->shape->getOutlineColor()==clickSecondOutlineColor || sl2->shape->getOutlineColor()==clickFirstOutlineColor){
                    for(int i=0;i<board[sl2->ind].size();i++){
                        if(list[board[sl2->ind][i]-1]->shape->getFillColor() == PlayerTwoFillColor){
                            valueOfSlot+=0.5;
                        }
                    }
                    for(int i=0;i<board[sl2->ind].size();i++){
                        if(list[board[sl2->ind][i]-1]->shape->getFillColor() == PlayerOneFillColor){
                            valueOfSlot+=1.2;
                        }
                    }
                    sv->value = valueOfSlot;
                    int czyJest = 0;
                    for(SlotValue *slotValue:moveValues){
                        if(slotValue->Slot == sv->Slot){
                            czyJest = 1;
                            if(sv->value > slotValue->value){
                                slotValue = sv;
                            }
                        }
                    }
                    if(!czyJest)
                        moveValues.push_back(sv);

                }
            }
        }
    }

    int maxPoints=0;
    SlotValue *pickedSlot;
    slot* clickedSlot=nullptr;


    for(int i=0;i<moveValues.size();i++){
        std::cout << i << ":" << moveValues[i]->value << ", ";
        if(moveValues.at(i)->value > maxPoints){
            maxPoints = moveValues.at(i)->value;
            pickedSlot = moveValues[i];
            if(moveValues.at(i)->clickedSlot != nullptr){
                clickedSlot = moveValues.at(i)->clickedSlot;
            }
            else{
                clickedSlot = nullptr;
                // PlayerTwoNumberOfPoints++;  
            }
        }
    }std::cout<<"\n";


    for(slot *sl:list){
        sl->shape->setOutlineColor(outlineColor);
    }

    pickedSlot->Slot->shape->setFillColor(PlayerTwoFillColor);
    pickedSlot->Slot->shape->setOutlineColor(sf::Color(255,0,0));
    if(clickedSlot!=nullptr)
        clickedSlot->shape->setFillColor(fillColor);
    colorEnemy(turn,pickedSlot->Slot->ind);

    turn=(turn+1)%2;
    std::cout << "turn: " << turn << "\n";
}


bool Board::PlayerMove(sf::RenderWindow &window, sf::Vector2i mousePos){
    ///zarzadza kliknieciem. Wywoluje metode click oraz zwaraca to samo co metoda click. zmienia ture na ture przeciwnika oraz czysci zaznaczenie wsyzstkich pol
    if(!click(window,mousePos))
        return 0;

    for(slot *sl:list){
        sl->shape->setOutlineColor(outlineColor);
    }

    turn=(turn+1)%2;
    std::cout << "turn: " << turn << "\n";

    return 1;
}



bool Board::click(sf::RenderWindow &window, sf::Vector2i mousePos){

    ///Pojedynczy ruch gracza w zaleznosci od tego kogo jest tura. W pierwszej kolejnosci zaznaca pola gdzie mozna sie ruszyc po kliknieciu na jeden ze swoich pionkokw a nastrepnei jesli sie kliknie na zaznaczone pole ruch sie wykonuje. Metoda zwraca 1 jeseli gracz kliknie na zaznaczone pole gdzie mozna sie ruszyc, w przeciwnym wypadku 0;

    slot *button = nullptr;
    for(slot *b:list){
        if (b->shape->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
            button = b;
        }
    }

    if(button==nullptr)
        return 0; 


    if(button->shape->getOutlineColor() == clickFirstOutlineColor){
        button->shape->setFillColor(turn==0?PlayerOneFillColor:PlayerTwoFillColor);
        // turn==0?PlayerOneNumberOfPoints++:PlayerTwoNumberOfPoints++;
        colorEnemy(turn,button->ind);
        return 1;
    } 
    if(button->shape->getOutlineColor() == clickSecondOutlineColor){
        button->shape->setFillColor(turn==0?PlayerOneFillColor:PlayerTwoFillColor);
        clickedSlot->shape->setFillColor(fillColor);
        colorEnemy(turn,button->ind);
        return 1;
    } 
    
    for(slot *sl:list){
        if(sl->shape->getFillColor()!= PlayerOneFillColor && sl->shape->getFillColor()!=PlayerTwoFillColor)
            sl->shape->setFillColor(fillColor);
        sl->shape->setOutlineColor(outlineColor);
    }


    if(button->shape->getFillColor()!=PlayerOneFillColor && turn!=1)
        return 0;

    if(button->shape->getFillColor()!=PlayerTwoFillColor && turn!=0)
        return 0;


    clickedSlot = button;

    while(!kolejka.empty())
        kolejka.pop();
    elementOfQueue element = {button->ind , 0};
    kolejka.push(element);
    BFS();

    return 0;
}

void Board::colorEnemy(int player, int ind){
    ///koloryje wszytkie pionki przeciwnika do okola danego pola na kolor gracza.

    for(int i=0;i<board[ind].size();i++){
        slot* sl = list.at(board[ind].at(i)-1);
        if(sl->shape->getFillColor() == (player==0?PlayerTwoFillColor:PlayerOneFillColor)){
            // turn==0?PlayerOneNumberOfPoints++:PlayerTwoNumberOfPoints++;
            // turn==1?PlayerOneNumberOfPoints--:PlayerTwoNumberOfPoints--;
            sl->shape->setFillColor(player==1?PlayerTwoFillColor:PlayerOneFillColor);
        }
    }
    
}

void Board::BFS(){
    /// za pomoca BFS przechodzi po palnszy do gry od danego elementu o 2 pola w kazda strone. jezeli element jest oddalony o jeden to koloruje na kolor ruchy o jeden a jezeli o 2 to kolor ruchu o dwa
    elementOfQueue element = kolejka.front();
    // std::cout << element.ind << " " << element.depth << "\n";
    kolejka.pop();

    
    slot *button = list.at(element.ind-1);
    if(button->shape->getOutlineColor() == outlineColor && button->shape->getFillColor()==fillColor){
        switch(element.depth){
            case 0:
                button->shape->setOutlineColor(clickCenterOutlineColor);
                break;
            case 1: 
                button->shape->setOutlineColor(clickFirstOutlineColor);
                break;
            case 2: 
                button->shape->setOutlineColor(clickSecondOutlineColor);
                break;
        }

    }

    if(element.depth<2){
        for(int i=0;i<board[element.ind].size();i++){
            slot* sl = list.at(board[element.ind].at(i)-1);
            // if(sl->shape->getOutlineColor() == outlineColor && sl->shape->getFillColor() == fillColor){
            //     elementOfQueue nextElement = {board[element.ind].at(i),element.depth+1};
            //     kolejka.push(nextElement);
            // }
            if(sl->shape->getOutlineColor() == outlineColor){
                elementOfQueue nextElement = {board[element.ind].at(i),element.depth+1};
                kolejka.push(nextElement);
            }
        }
    }
    

    if(!kolejka.empty())
        BFS();
}

void Board::setSlots(){
    /// uzupelnie liste list ktora przchowuje po kolei pola w takiej kolejnosci jak pokazuje zakomendowany testk na poczatku klassy. Tworzy te pola kolorujac je na odpowiedni kolor , ustala ich polozenia oraz nadaje im indeksy po kolei od 1 do 61

    float x = 500;
    float y = 50;
    int level = 1;
    int inRow = 1;

    // slot* firstPoint = new slot();
    // list.push_back(firstPoint);

    for(int i=1;i<=15;i++){
        slot *point = new slot();

        point->shape = new sf::CircleShape(45,6);
        point->shape->rotate(90);
        point->shape->setFillColor(fillColor);
        point->shape->setOutlineThickness(5);
        point->shape->setOutlineColor(outlineColor);
        point->ind = i;
        point->shape->setPosition({x,y});

        if(inRow < level){
            x += 176;
            inRow++;
        }else{
            y += 54;
            x -= (176*(level)) - 88;
            inRow = 1;
            level++;
        }


        list.push_back(point);
        
    }

    x+=176;
    float leftSite = x;

    for(int i=16;i<=51;i++){
        slot *point = new slot();

        point->shape = new sf::CircleShape(45,6);
        point->shape->rotate(90);
        point->shape->setFillColor(fillColor);
        point->shape->setOutlineThickness(5);
        point->shape->setOutlineColor(outlineColor);
        point->ind = i;
        point->shape->setPosition({x,y});

        if(level%2 == 0){
            if(inRow == 4){
                x = leftSite - 88;
                y += 54;
                level++;
                inRow = 1;
            }else{
                x+=176;
                inRow++;
            }
        }else{
            if(inRow == 5){
                x = leftSite;
                y += 54;
                level++;
                inRow = 1;
            }else{
                x+=176;
                inRow++;
            }
        }

        // if(i!=22 && i!=36 && i!=35){
        list.push_back(point);
        // }

        


    }

    level = 4;

    for(int i=52;i<=61;i++){
        slot *point = new slot();

        point->shape = new sf::CircleShape(45,6);
        point->shape->rotate(90);
        point->shape->setFillColor(fillColor);
        point->shape->setOutlineThickness(5);
        point->shape->setOutlineColor(outlineColor);
        point->ind = i;
        point->shape->setPosition({x,y});

        if(inRow < level){
            x += 176;
            inRow++;
        }else{
            y += 54;
            x -= (176*(level-2)) + 88;
            inRow = 1;
            level--;
        }

        list.push_back(point);
    }

    list.at(0)->shape->setFillColor(PlayerOneFillColor);
    list.at(50)->shape->setFillColor(PlayerOneFillColor);
    list.at(46)->shape->setFillColor(PlayerOneFillColor);

    list.at(10)->shape->setFillColor(PlayerTwoFillColor);
    list.at(14)->shape->setFillColor(PlayerTwoFillColor);
    list.at(60)->shape->setFillColor(PlayerTwoFillColor);
}

void Board::draw(sf::RenderWindow &window) const {
    ///rysuej wszystkie pola  z listy 
    // std::cout << "rozmiar: " << list.size() << "\n";
    window.clear();
    for(slot *sl:list){
        // std::cout << sl->ind << "\n";
        int i = sl->ind;
        if(i!=22 && i!=36 && i!=35){
            window.draw(*(sl->shape));
        }
    }
}

void Board::setGameStage(Stage stage){
    gameStage = stage;
}

int Board::getNumberOfPointsPlayerOne()const{
    int points = 0;
    for(slot* sl:list){
        if(sl->shape->getFillColor() == PlayerOneFillColor)
            points++;
    }
    return points;
}

int Board::getNumberOfPointsPlayerTwo()const{
    int points = 0;
    for(slot* sl:list){
        if(sl->shape->getFillColor() == PlayerTwoFillColor)
            points++;
    }
    return points;
}

bool Board::isEnd(int player){
    ///1 jezeli jest koniec, 0 jezeli nie 
    
    bool isAvaibleSlot = 0;

    if(getNumberOfPointsPlayerOne()+getNumberOfPointsPlayerTwo() == 61)
        return 1;

    for(slot* sl:list){
        if(sl->shape->getFillColor()==(player==0?PlayerTwoFillColor:PlayerOneFillColor)){
            for(int i=0;i<board[sl->ind].size();i++){
                slot* sl2 = list.at(board[sl->ind].at(i)-1);
                for(int j=0;j<board[sl2->ind].size();j++){
                    slot* sl3 = list.at(board[sl2->ind].at(j)-1);
                    if(sl3->shape->getFillColor() == fillColor){
                        isAvaibleSlot = 1;
                    }
                }
            }
        }
    }

    if(isAvaibleSlot == 1)
        return 0;
    else
        return 1;

}

int Board::getWinningPlayer()const{
    if(getNumberOfPointsPlayerOne()==getNumberOfPointsPlayerTwo())
        return 0;
    if(getNumberOfPointsPlayerOne()>getNumberOfPointsPlayerTwo())
        return 1;
    return -1;
}

std::vector<slot*> Board::getList()const{
    return list;
}

int Board::getTurn()const{
    return turn;
}

void Board::loadGame(){

    ///koloruje wsyzstkie pola na kolor podany w pliku savedGame.txt, ustawia czyja jest tura oz ustawie tryb gry
    
    std::ifstream savedGame("savedGame.txt");
    int color;
    int i=-1;

    while(savedGame >> color){
        i++;
        if(i == 61){
            turn = color;
            std::cout << "loaded turn: " << turn <<"\n";
            continue;
        }
        if(i == 62){
            if(color == 0)
                gameStage = PVPGame;
            else 
                gameStage= PVEGame;
            std::cout << "loaded game mode: " << gameStage << "\n";
            break;
        }
        switch(color){
            case 0:
                list[i]->shape->setFillColor(fillColor);
                break;
            case 1:
                list[i]->shape->setFillColor(PlayerOneFillColor);
                break;
            case 2:
                list[i]->shape->setFillColor(PlayerTwoFillColor);
                break;
        }
    }

}

Stage Board::getStage()const{
    /// getter dla gameStage
    return gameStage;
}

void Board::writeTopScores(){
    ///Pod koniec kazdej gry Tworzy plik topScores.txt jezeli go nie ma. dopisuje do pliku wynik do puki nie ma ich tam 5. jezeli jest juz 5 wynikow to nadpisuje najmniejszy z nich ktory jest mniejszy od wyniku pod koniec danej gry.
    // std::fstream topScores;
    std::ifstream topScores("topScores.txt");

    int p1,p2;
    int tab[5][2];
    int ii=-1;

    int newP1 = getNumberOfPointsPlayerOne();
    int newP2 = getNumberOfPointsPlayerTwo();

    

    if(topScores){
        while(topScores >> p1){
            ii++;
            topScores >> p2;
            tab[ii][0] = p1;
            tab[ii][1] = p2;
            std::cout << tab[ii][0] << " " << tab[ii][1] << "\n";
        }
        topScores.close();
        
        if(ii < 4){
            std::cout << "score saved 1\n";
            std::ofstream topScores("topScores.txt", std::ios_base::app);
            std::string textToAppend = "\n" + std::to_string(newP1) + " " + std::to_string(newP2);
            topScores << textToAppend;
            topScores.close();
        }else {
            std::cout << "score saved 2\n";
            int ind=-1;
            int topScore = 63;

            for(int i=0;i<5;i++){
                if(newP1 > tab[i][0]){
                    if(tab[i][0] < topScore){
                        topScore = tab[i][0];
                        ind = i;
                    }
                }
            }


            if(ind!=-1){
                tab[ind][0] = newP1;
                tab[ind][1] = newP2;
            }

            topScores.close();
            std::ofstream topScores("topScores.txt");
            std::string textToAppend = "";
            for(int i=0;i<5;i++){
                std::cout << tab[i][0] << " " << tab[i][1] << "\n";
                textToAppend+=std::to_string(tab[i][0]) + " " + std::to_string(tab[i][1]) + "\n";
            }
            topScores << textToAppend;
            topScores.close();
        }
    } else {
        std::cout << "score saved 3\n";
        std::ofstream topScores("topScores.txt");
        std::string textToAppend = "\n" + std::to_string(newP1) + " " + std::to_string(newP2);
        topScores << textToAppend;
        topScores.close();
    }


}
