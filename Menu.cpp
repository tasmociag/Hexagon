#include <iostream>
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <queue>
#include <vector>
#include <fstream>
#include "Menu.h"
#include "Board.h"

Menu::Menu(){

    stage = Start;
    board = new Board();

    if (!font.loadFromFile("NotoMono-Regular.ttf")) {
        std::cout << "Failed to load font." << std::endl;
        return;
    }
    
    PlayGame->setFont(font);
    PlayGame->setString("Play Game");
    PlayGame->setCharacterSize(100);
    PlayGame->setPosition({190, 250});
    PlayGame->setFillColor(fillColor);

    LoadGame->setFont(font);
    LoadGame->setString("Load Last\n  Game");
    LoadGame->setCharacterSize(100);
    LoadGame->setPosition({190, 500});
    LoadGame->setFillColor(fillColor);

    PlayerVsPlayer->setFont(font);
    PlayerVsPlayer->setString("Player\n  VS\nPlayer");
    PlayerVsPlayer->setCharacterSize(100);
    PlayerVsPlayer->setPosition({270, 80});
    PlayerVsPlayer->setFillColor(fillColor);

    PlayerVsBot->setFont(font);
    PlayerVsBot->setString("Player\n  VS\n Bot");
    PlayerVsBot->setCharacterSize(100);
    PlayerVsBot->setPosition({270, 620});
    PlayerVsBot->setFillColor(fillColor);

    ReturnToMenu->setFont(font);
    ReturnToMenu->setString("Return\nto menu");
    ReturnToMenu->setCharacterSize(40);
    ReturnToMenu->setPosition({50, 950});
    ReturnToMenu->setFillColor(sf::Color(64,224,208));

    PlayerOnePoints->setFont(font);
    PlayerOnePoints->setString("Player one points = 0");
    PlayerOnePoints->setCharacterSize(20);
    PlayerOnePoints->setPosition({600, 50});
    PlayerOnePoints->setFillColor(sf::Color(26,255,0));
    
    PlayerTwoPoints->setFont(font);
    PlayerTwoPoints->setString("Player two points = 0");
    PlayerTwoPoints->setCharacterSize(20);
    PlayerTwoPoints->setPosition({600, 80});
    PlayerTwoPoints->setFillColor(sf::Color(222,0,255));

    Back->setFont(font);
    Back->setString("Back");
    Back->setCharacterSize(40);
    Back->setPosition({50, 950});
    Back->setFillColor(sf::Color(64,224,208));

    Exit->setFont(font);
    Exit->setString("Exit");
    Exit->setCharacterSize(40);
    Exit->setPosition({50, 950});
    Exit->setFillColor(sf::Color(64,224,208));

    GameOverText->setFont(font);
    GameOverText->setString("GameOverText");
    GameOverText->setCharacterSize(120);
    GameOverText->setPosition({100,250});
    GameOverText->setFillColor(sf::Color(255,0,0));
    GameOverText->setOutlineColor(sf::Color::Black);
    GameOverText->setOutlineThickness(7);

    SaveGameText->setFont(font);
    SaveGameText->setString("Save\nGame");
    SaveGameText->setCharacterSize(40);
    SaveGameText->setPosition({750, 950});
    SaveGameText->setFillColor(sf::Color(64,224,208));

    GameSavedText->setFont(font);
    GameSavedText->setString("Game\nSaved");
    GameSavedText->setCharacterSize(90);
    GameSavedText->setPosition({300,400});
    GameSavedText->setFillColor(sf::Color(0,0,255));
    GameSavedText->setOutlineColor(sf::Color::Black);
    GameSavedText->setOutlineThickness(7);

    NoSavedGameFile->setFont(font);
    NoSavedGameFile->setString("No Saved\nGame File");
    NoSavedGameFile->setCharacterSize(90);
    NoSavedGameFile->setPosition({300,400});
    NoSavedGameFile->setFillColor(sf::Color(255,0,0));
    NoSavedGameFile->setOutlineColor(sf::Color::Black);
    NoSavedGameFile->setOutlineThickness(7);

    


    
    PlayButton->setSize(sf::Vector2f(560, 140));
    PlayButton->setPosition({180, 250});
    PlayButton->setFillColor(sf::Color::Blue);

    LoadButton->setSize(sf::Vector2f(560, 260));
    LoadButton->setPosition({180, 500});
    LoadButton->setFillColor(sf::Color::Blue);

    PlayerVsPlayerButton->setSize(sf::Vector2f(374, 380));
    PlayerVsPlayerButton->setPosition({260, 80});
    PlayerVsPlayerButton->setFillColor(sf::Color::Blue);

    PlayerVsBotButton->setSize(sf::Vector2f(374, 380));
    PlayerVsBotButton->setPosition({260, 620});
    PlayerVsBotButton->setFillColor(sf::Color::Blue);

    ReturnToMenuButton->setSize(sf::Vector2f(190, 120));
    ReturnToMenuButton->setPosition({40, 940});
    ReturnToMenuButton->setFillColor(sf::Color::Blue);

    BackButton->setSize(sf::Vector2f(120, 70));
    BackButton->setPosition({40, 940});
    BackButton->setFillColor(sf::Color::Blue);

    ExitButton->setSize(sf::Vector2f(190, 90));
    ExitButton->setPosition({40, 940});
    ExitButton->setFillColor(sf::Color::Blue);

    SaveGameButton->setSize(sf::Vector2f(130, 120));
    SaveGameButton->setPosition({740, 940});
    SaveGameButton->setFillColor(sf::Color::Blue);

}

void Menu::clickAction(sf::RenderWindow &window, sf::Vector2i mousePos){
    ///zadzadza kazdym zdarzeniem klikniecia w zaleznosci jaki jest stan gry
    switch(stage){
        case Start: 
            clickMenuMenu(window,mousePos);
            clickExit(window,mousePos);
            break; 

        case Play: 
            clickPlayMenu(window,mousePos);
            clickBack(mousePos);
            break; 

        case PVPGame: 
            if(board->Turn(window,mousePos)){
                draw(window);
                stage = GameOver;
                board->writeTopScores();
                break;
            }
            clickReturnToMenu(mousePos);
            clickSaveGame(window,mousePos);
            break; 

        case PVEGame: 
            if(board->Turn(window,mousePos)){
                draw(window);
                stage = GameOver;
                break;
            }
            clickReturnToMenu(mousePos);
            clickSaveGame(window,mousePos);
            break; 

        case GameOver:

            clickReturnToMenu(mousePos);

            break;

    }
}

void Menu::clickSaveGame(sf::RenderWindow &window, sf::Vector2i mousePos){
    ///sprawdza czy zostal klikniety guzik saveGame, jezeli tak to zapisuje gre
    if (SaveGameButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
        window.draw(*GameSavedText);
        window.display();
        std::string toWrite = "";

        std::ofstream savedGame("savedGame.txt");
        std::vector<slot*> vec = board->getList();
        for(slot* sl:vec){
            if(sl->shape->getFillColor()==fillColor)
                toWrite+="0 ";
            if(sl->shape->getFillColor()==PlayerOneFillColor)
                toWrite+="1 ";
            if(sl->shape->getFillColor()==PlayerTwoFillColor)
                toWrite+="2 ";
        
        }
        toWrite+="\n"+std::to_string(board->getTurn());
        if(stage == PVPGame)
            toWrite+=" 0";
        else
            toWrite+=" 1";
        savedGame << toWrite;
        savedGame.close();
        

        sleep(1);
    }
}

void Menu::clickReturnToMenu(sf::Vector2i mousePos){
    ///sprawdza czy zostal klikniety guzik ReturnToMenu jezeli tak to zmienia stage do na Menu
    if (ReturnToMenuButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
        stage = Start;
    }
}

void Menu::clickExit(sf::RenderWindow &window, sf::Vector2i mousePos){
    ///sprawdza czy zostal klikniety guzik Exit, jezeli tak to zamyka sie okno
    if (ExitButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
        window.close();
    }
}

void Menu::clickBack(sf::Vector2i mousePos){
    ///sprawdza czy zostal klikniety guzik Back, jezeli tak to wraca do poprzenidego menu
    if (BackButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
        stage = Start;
    }
}

void Menu::clickPlayMenu(sf::RenderWindow &window, sf::Vector2i mousePos){

    ///sprawdza ktry tryb gry zostal wybrany i rozpoczyna rozgrywke

    if (PlayerVsPlayerButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
        // drawPlayMenu(window);
        stage = PVPGame;
        board->setGameStage(stage);
    }

    if (PlayerVsBotButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
        // drawPlayMenu(window);
        stage = PVEGame;
        board->setGameStage(stage);
    }
}


void Menu::clickMenuMenu(sf::RenderWindow &window, sf::Vector2i mousePos){
    ///sprawdza czy zostal klikniety guzik w menu glowynm oraz przenosi do menu wybory trybu gry lub zaladownia ostatniej rozgrywki

    if (PlayButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {
        board = new Board();
        stage = Play;
    }

    if (LoadButton->getGlobalBounds().contains(mousePos.x,mousePos.y)) {

        std::cout << "load";
        std::ifstream savedGame("savedGame.txt");
        if(!savedGame){
            std::cout << "no load Game";
            window.draw(*NoSavedGameFile);
            window.display();
            sleep(1);
            draw(window);
            savedGame.close();
        }else{
            savedGame.close();
            board = new Board();
            board->loadGame();
            std::cout << "stage: " << stage << "\n";
            stage = board->getStage();
        }



    }
}

void Menu::drawMenu(sf::RenderWindow &window)const{
    ///rysuje menu glowne
    window.clear();
    // window.draw(*PlayButton);
    // window.draw(*LoadButton);
    window.draw(*PlayGame);
    window.draw(*LoadGame);
    window.draw(*Exit);
}

void Menu::drawPlayMenu(sf::RenderWindow &window)const{
    ///rysuje menu wyboru trybu gry 
    window.clear();
    // window.draw(*PlayerVsPlayerButton);
    // window.draw(*PlayerVsBotButton);
    window.draw(*PlayerVsPlayer);
    window.draw(*PlayerVsBot);
}

void Menu::drawGameOver(sf::RenderWindow &window)const{
    ///rysuje napis game over

    if(board->getWinningPlayer() == 1){
        GameOverText->setPosition({235,250});
        GameOverText->setString("Player\nOne\nwon!!");
    }else if(board->getWinningPlayer() == -1){
        GameOverText->setPosition({235,250});
        GameOverText->setString("Player\nTwo\nwon!!");
    }else {
        GameOverText->setString("Tie");
    }
    // window.draw(*EndOfGameBox);
    window.draw(*GameOverText);
}

void Menu::draw(sf::RenderWindow &window){
    ///odpowiada za rysowanie odpowiedniego ekranu w zaleznosci od tego jaki jest stan gry
    switch(stage){
        case Start: 
            drawMenu(window);
            break; 

        case Play: 
            drawPlayMenu(window);
            // window.draw(*BackButton);
            window.draw(*Back);
            break; 


        case PVPGame: 
            board->draw(window);
            window.draw(*ReturnToMenu);
            PlayerOnePoints->setString("Player One points = "+std::to_string(board->getNumberOfPointsPlayerOne()));
            PlayerTwoPoints->setString("Player Two points = "+std::to_string(board->getNumberOfPointsPlayerTwo()));
            window.draw(*PlayerOnePoints);
            window.draw(*PlayerTwoPoints);
            window.draw(*SaveGameText);
            break; 

        case PVEGame: 
            board->draw(window);
            window.draw(*ReturnToMenu);
            PlayerOnePoints->setString("Player One points = "+std::to_string(board->getNumberOfPointsPlayerOne()));
            PlayerTwoPoints->setString("Player Two points = "+std::to_string(board->getNumberOfPointsPlayerTwo()));
            window.draw(*PlayerOnePoints);
            window.draw(*PlayerTwoPoints);
            window.draw(*SaveGameText);
            break; 

        case GameOver: 
            board->draw(window);
            window.draw(*ReturnToMenu);
            drawGameOver(window);
            break;
    }

}
