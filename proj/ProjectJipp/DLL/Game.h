#ifndef GAME_H
#define GAME_H

#include "API.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include "Stock.h"
#include "GameState.h"

class EXPORT_API Game {
public:
    Game();
    void loadResources();
    void gameLoop();
    void setDraggingState(bool state);
    void setDraggingStateCards(bool state);
    bool isAnyCardBeingDragged() const;

private:

    sf::Sprite m_title;
    sf::Texture m_titleTexture;

    //0
    sf::Sprite m_mainMenuNg;
    //1
    sf::Sprite m_mainMenuLg;
    //2
    sf::Sprite m_gameMenuPs;
    //3
    sf::Sprite m_pauseMenuRs;
    //4
    sf::Sprite m_pauseMenuEm;
    //5
    sf::Sprite m_mainMenuQt;

    sf::Texture m_buttonTextures[6];

    GameState m_state;
    sf::RenderWindow window;
    Stock stocks[7];
    Stock finishStocks[4];
    
    Stock withdrawStock;
    sf::RectangleShape withdrawRect;

    Stock showingStock;
    Card* draggingCard;
    std::vector<Card*> draggingCards;
    bool isDragging;
    bool isDraggingCards;

    bool isGameWon;

    int numOfStocks;
    float cardSizeX;
    float cardSizeY;
    float distX;
    float distY;
    float startPosX;
    float startPosY;
    float finishPosX;
    float finishPosY;
    float withdrawPosX;
    float withdrawPosY;
    void resetGame();
    void setCardsOnStart();
    void processEvents();
    void update();
    void render();
};

#endif // GAME_H
