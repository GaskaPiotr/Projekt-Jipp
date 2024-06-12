#include "Game.h"
#include "Card.h"
#include <iostream>

Game::Game() : m_state(GameState::Loading), window(sf::VideoMode(1400, 1000), "Solitaire"), isGameWon(false),  isDragging(false), isDraggingCards(false), draggingCard(nullptr), numOfStocks(7), cardSizeX(112.f), cardSizeY(156.f), distX(150.f), distY(30.f), startPosX(100.f), startPosY(300.f), finishPosX(100.f), finishPosY(100.f), withdrawPosX(1200.f), withdrawPosY(100.f), withdrawRect(sf::Vector2f(105.75f, 151.3125f)) {

}

void Game::gameLoop() {
    while (window.isOpen()) {

        window.clear(sf::Color(24, 175, 49));
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        switch (m_state)
        {

        case Loading:
            loadResources();
            m_state = MainMenu;
            break;

        case MainMenu:
            window.draw(m_title);
            window.draw(m_mainMenuNg);
            window.draw(m_mainMenuLg);
            window.draw(m_mainMenuQt);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (m_mainMenuNg.getGlobalBounds().contains(pos))
                {
                    resetGame();
                    
                    m_state = LevelLoading;
                }

                else if (m_mainMenuLg.getGlobalBounds().contains(pos)) {
                    bool gameIsLoaded = false;
                    for (int i = 0; i < numOfStocks; i++) {
                        if (stocks[i].getTopCardPtr()) {
                            gameIsLoaded = true;
                        }
                    }
                    if (gameIsLoaded) {
                        if (isGameWon) {
                            resetGame();
                            m_state = LevelLoading;
                        }
                        else {
                            m_state = Playing;
                        }
                    }
                    else {
                        resetGame();
                        m_state = LevelLoading;
                    }
                }
                else if (m_mainMenuQt.getGlobalBounds().contains(pos))
                {
                    window.close();
                }
            }
            break;

        case LevelLoading:
            setCardsOnStart();
            m_state = Playing;
            break;

        case Playing:
            window.draw(m_gameMenuPs);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (m_gameMenuPs.getGlobalBounds().contains(pos))
                {
                    m_state = Pause;
                }
            }

            if (isGameWon) {
                m_state = MainMenu;
            }

            processEvents();
            update();
            render();
            break;

        case Pause:
            window.draw(m_pauseMenuRs);
            window.draw(m_pauseMenuEm);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2f pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (m_pauseMenuRs.getGlobalBounds().contains(pos))
                {
                    m_state = Playing;
                }
                else if (m_pauseMenuEm.getGlobalBounds().contains(pos))
                {
                    sf::sleep(sf::milliseconds(200));
                    m_state = Loading;
                }
            }
            break;

        }
        window.display();
        
    }
}

void Game::resetGame() {
    for (int i = 0; i < numOfStocks; i++) {
        for (Card* card : stocks[i].getCards()) {
            delete card; // Usuwanie dynamicznie alokowanych 
        }
        while (stocks[i].getTopCardPtr()) {
            stocks[i].removeTopCard();
        }
    }
    for (int i = 0; i < 4; i++) {
        for (Card* card : finishStocks[i].getCards()) {
            delete card; // Usuwanie dynamicznie alokowanych kart
        }
        while (finishStocks[i].getTopCardPtr()) {
            finishStocks[i].removeTopCard();
        }
    }
    for (Card* card : withdrawStock.getCards()) {
        delete card; // Usuwanie dynamicznie alokowanych kart
    }
    while (withdrawStock.getTopCardPtr()) {
        withdrawStock.removeTopCard();
    }
    for (Card* card : showingStock.getCards()) {
        delete card; // Usuwanie dynamicznie alokowanych kart
    }
    while (showingStock.getTopCardPtr()) {
        showingStock.removeTopCard();
    }
    isGameWon = false;
}


void Game::loadResources()
{
    m_titleTexture.loadFromFile("menuTextures/Tytu³.png");
    m_title.setTexture(m_titleTexture);
    m_title.setPosition(286.5f, 0);


    m_buttonTextures[0].loadFromFile("menuTextures/New Game.png");
    m_buttonTextures[1].loadFromFile("menuTextures/Load Game.png");
    m_buttonTextures[2].loadFromFile("menuTextures/Pause.png");
    m_buttonTextures[3].loadFromFile("menuTextures/Resume Game.png");
    m_buttonTextures[4].loadFromFile("menuTextures/Exit to Main Menu.png");
    m_buttonTextures[5].loadFromFile("menuTextures/Quit Game.png");


    m_mainMenuNg.setTexture(m_buttonTextures[0]);
    m_mainMenuLg.setTexture(m_buttonTextures[1]);
    m_gameMenuPs.setTexture(m_buttonTextures[2]);
    m_pauseMenuRs.setTexture(m_buttonTextures[3]);
    m_pauseMenuEm.setTexture(m_buttonTextures[4]);
    m_mainMenuQt.setTexture(m_buttonTextures[5]);


    m_mainMenuNg.setPosition(490.5f, 220);
    m_mainMenuLg.setPosition(463, 440);
    m_mainMenuQt.setPosition(433, 660);



    m_gameMenuPs.setPosition(1300, 0);

    m_pauseMenuRs.setPosition(404.5f, 100);
    m_pauseMenuEm.setPosition(314.5f, 500);
}


void Game::setDraggingState(bool state) {
    isDragging = state;
}

void Game::setDraggingStateCards(bool state) {
    isDraggingCards = state;
}

bool Game::isAnyCardBeingDragged() const {
    return isDragging;
}

void Game::setCardsOnStart() {
    //Dodanie wszystkich kart do odpowiednich stosow w sposob losowy
    std::vector<Card*> cards;
    std::vector<int> numsOfCards;
    for (int i = 1; i <= 13; i++) {
        for (int j = 0; j < 4; j++) {
            cards.push_back(new Card(cardSizeX, cardSizeY, i, j));
        }
    }
    srand((int)time(0));
    int random;
    int doubleRandom;
    for (int i = 0; i < numOfStocks; i++) {
        for (int j = i; j < numOfStocks; j++) {
            random = rand() % 52;
            bool isIn = false;
            for (int k : numsOfCards) {
                if (random == k) {
                    isIn = true;
                    break;
                }
            }

            //Jesli randomowy indeks sie powtorzyl
            if (isIn) {
                doubleRandom = rand() % 2;
                int change = 0;
                if (doubleRandom) {
                    change = 1;
                }
                else {
                    change = -1;
                }
                while (isIn) {
                    random += change;
                    if (random < 0) {
                        random = 51;
                    }
                    else if (random > 51) {
                        random = 0;
                    }
                    for (int k : numsOfCards) {
                        if (random == k) {
                            break;
                        }
                        else if (numsOfCards.back() == k) {
                            isIn = false;
                        }
                    }
                }
                 
            }
            //Mamy randomowy indeks ktory nie byl wczesniej
            stocks[j].addCard(cards[random]);
            numsOfCards.push_back(random);
        }
    }

    //Dodanie reszty kart na stos do zabierania
    for (int i = 0; i < 52; i++) {
        bool isIn = false;
        for (int k : numsOfCards) {
            if (k == i) {
                isIn = true;
                break;
            }
        }
        if (!isIn) {
            withdrawStock.addCard(cards[i]);
        }
    }


    //Ustawianie 7 stosow do gry 
    for (int i = 0; i < numOfStocks; i++) {
        int counter = 0;
        stocks[i].setPosition(startPosX + distX * i, startPosY);
        sf::RectangleShape rect(sf::Vector2f(cardSizeX, cardSizeY));
        rect.setOrigin(cardSizeX / 2.0f, cardSizeY / 2.0f);
        rect.setPosition(startPosX + distX * i, startPosY);
        rect.setFillColor(sf::Color(71,79,69));
        stocks[i].magicRect = rect;
        std::vector<Card*>& cards = stocks[i].getCards();
        for (Card* card : cards) {
            float posX = startPosX + distX * i;
            float posY = startPosY + distY * counter;
            card->setPositionOnStock(posX, posY);
            card->getShape().setTexture(&card->getBackTexture());
            card->setLastStock(i);
            counter++;
        }
        if (stocks[i].getTopCardPtr()) {
            stocks[i].getTopCardPtr()->setIsOnWithdraw(false);
            stocks[i].getTopCardPtr()->renderTexture();
        }
    }

    //Ustawianie 4 stosow do wygrania
    for (int i = 0; i < 4; i++) {
        finishStocks[i].setPosition(finishPosX + distX * i, finishPosY);
        sf::RectangleShape rect(sf::Vector2f(cardSizeX, cardSizeY));
        rect.setOrigin(cardSizeX / 2.0f, cardSizeY / 2.0f);
        rect.setPosition(finishPosX + distX * i, finishPosY);
        rect.setFillColor(sf::Color(71, 79, 69));
        finishStocks[i].magicRect = rect;
    }


    //Ustawienie stosu zabierania
    withdrawStock.setPosition(withdrawPosX, withdrawPosY);
    sf::RectangleShape rectW(sf::Vector2f(cardSizeX, cardSizeY));
    rectW.setOrigin(cardSizeX / 2.0f, cardSizeY / 2.0f);
    rectW.setPosition(withdrawPosX, withdrawPosY);
    rectW.setFillColor(sf::Color(71, 79, 69));
    withdrawStock.magicRect = rectW;


    //std::vector<Card*>& cards = withdrawStock.getCards();
    for (Card* card : withdrawStock.getCards()) {
        float posX = withdrawPosX;
        float posY = withdrawPosY;
        card->setPositionOnStock(posX, posY);
        card->getShape().setTexture(&card->getBackTexture());
        card->setLastStock(numOfStocks + 4);
    }


    
    //Ustawienie stosu pokazywania
    showingStock.setPosition(withdrawPosX - distX, withdrawPosY);
    sf::RectangleShape rectS(sf::Vector2f(cardSizeX, cardSizeY));
    rectS.setOrigin(cardSizeX / 2.0f, cardSizeY / 2.0f);
    rectS.setPosition(withdrawPosX - distX, withdrawPosY);
    rectS.setFillColor(sf::Color(71, 79, 69));
    showingStock.magicRect = rectS;


    render();
}

void Game::processEvents() {
    
    //Wziecie karty badz kart ze stosu
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !isAnyCardBeingDragged()){
        //Ze stosow
        for (int i = 0; i < numOfStocks; ++i) {
            Card* card = nullptr;
            card = stocks[i].whichCardIsMouseOver(window);

            if (card != nullptr && !card->getIsOnWithdraw()) {
                draggingCard = card;
                draggingCard->setIsOnWithdraw(false);

                //Wiecej kart 
                if (!(card == stocks[i].getTopCardPtr())) {
                    draggingCards = stocks[i].getCardsBelow(card);
                    stocks[i].removeCardsBelow(draggingCards.size() + 1);
                    setDraggingStateCards(true);
                }
                //Jedna karta
                else {
                    stocks[i].removeTopCard();
                }
                setDraggingState(true);
                break;
            }
        }
        //Z wygrywajacych stosow
        for (int i = 0; i < 4; ++i) {
            Card* card = nullptr;
            card = finishStocks[i].whichCardIsMouseOver(window);

            if (card != nullptr) {
                draggingCard = card;

                //Wiecej kart 
                if ((card == finishStocks[i].getTopCardPtr())) {
                    finishStocks[i].removeTopCard();
                }
                setDraggingState(true);
                break;
            }
        }

        //Ze stosu zabierania

        if (withdrawStock.isMouseOverLastCard(window) && withdrawStock.getTopCardPtr()) {
            draggingCard = withdrawStock.getBottomCardPtr();
            draggingCard->setIsOnWithdraw(false);
            draggingCard->renderTexture();
            withdrawStock.removeBottomCard();

            setDraggingState(true);
        }

        //Ze stosu pokazywania
        if (showingStock.isMouseOverLastCard(window) && showingStock.getTopCardPtr()) {
            draggingCard = showingStock.getTopCardPtr();
            draggingCard->renderTexture();
            showingStock.removeTopCard();

            setDraggingState(true);
        }
    }


    //Zwrocenie karty badz kart na stos i renderowanie tekstur kart i sprawdzenie czy gra jest wygrana
    else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && isAnyCardBeingDragged()){
        //Zwrocenie karty badz kart na stos 
        bool addedToStock = false;
        for (int i = 0; i < numOfStocks; ++i) {

            //Zwrocenie na nowy stos
            if (stocks[i].isMouseOverLastCard(window)) {
                float posX = 0;
                float posY = 0;

                //Stos nie jest pusty
                if (!stocks[i].getCards().empty()) {
                    //Sprawdzenie czy kolor kart sie zgadza do postawienia karty
                    if ((stocks[i].getTopCardPtr()->getColorOfCard() % 2) == (draggingCard->getColorOfCard() % 2)) {
                        break;
                    }
                    //Sprawdzenie czy numer karty sie zgadza do postawienia karty
                    else if (stocks[i].getTopCardPtr()->getNumberOfCard() != draggingCard->getNumberOfCard() + 1) {
                        break;
                    }
                    posX = stocks[i].getTopCardPtr()->getPositionOnStockX();
                    posY = stocks[i].getTopCardPtr()->getPositionOnStockY() + distY;
                }

                //Stos pusty
                else {
                    if (draggingCard->getNumberOfCard() != 13) {
                        break;
                    }
                    posX = stocks[i].getPositionX();
                    posY = stocks[i].getPositionY();
                }
                stocks[i].addCard(draggingCard);
                draggingCard->setLastStock(i);
                draggingCard->setPositionOnStock(posX, posY);
                if (isDraggingCards) {
                    int j = 0;
                    for (Card* c : draggingCards) {

                        stocks[i].addCard(c);
                        c->setLastStock(i);
                        c->setPositionOnStock(posX, posY + (j + 1) * distY);
                        j++;
                    }
                    draggingCards.clear();
                }
                draggingCard = nullptr;
                addedToStock = true;
                break;
            }
        }

        //Zwrocenie na wygrywajacy stos
        for (int i = 0; i < 4; ++i) {
            if (finishStocks[i].isMouseOverLastCard(window) && !isDraggingCards) {
                float posX = 0;
                float posY = 0;

                //Stos nie jest pusty
                if (!finishStocks[i].getCards().empty()) {
                    //Sprawdzenie czy kolor kart sie zgadza do postawienia karty
                    if (finishStocks[i].getTopCardPtr()->getColorOfCard() != draggingCard->getColorOfCard()) {
                        break;
                    }
                    //Sprawdzenie czy numer karty sie zgadza do postawienia karty
                    else if (finishStocks[i].getTopCardPtr()->getNumberOfCard() + 1 != draggingCard->getNumberOfCard()) {
                        break;
                    }
                    posX = finishStocks[i].getTopCardPtr()->getPositionOnStockX();
                    posY = finishStocks[i].getTopCardPtr()->getPositionOnStockY();
                }

                //Stos pusty
                else {
                    if (draggingCard->getNumberOfCard() != 1) {
                        break;
                    }
                    posX = finishStocks[i].getPositionX();
                    posY = finishStocks[i].getPositionY();
                }
                finishStocks[i].addCard(draggingCard);
                draggingCard->setLastStock(i + numOfStocks);
                draggingCard->setPositionOnStock(posX, posY);

                draggingCard = nullptr;
                addedToStock = true;
                break;
            }
        }

        //Zwracanie na stos pokazujacy
        if (showingStock.isMouseOverLastCard(window) && !isDraggingCards && draggingCard->getLastStock() == numOfStocks + 4) {
            float posX = showingStock.getPositionX();
            float posY = showingStock.getPositionY();

            //Stos nie jest pusty
            if (!showingStock.getCards().empty()) {
                Card* card = showingStock.getTopCardPtr();
                withdrawStock.addCard(card);
                card->setLastStock(numOfStocks + 4);
                card->setPositionOnStock(posX + distX, posY);
                card->setIsOnWithdraw(true);
                card->renderTexture();
                showingStock.removeTopCard();
            }
            showingStock.addCard(draggingCard);
            draggingCard->setLastStock(numOfStocks + 5);
            draggingCard->setPositionOnStock(posX, posY);

            draggingCard = nullptr;
            addedToStock = true;
        }

        //Zwrocenie na poprzedni stos
        if (!addedToStock) {
            if (draggingCard->getLastStock() < numOfStocks) {
                stocks[draggingCard->getLastStock()].addCard(draggingCard);
            }
            else if (draggingCard->getLastStock() >= numOfStocks && draggingCard->getLastStock() < numOfStocks + 4) {
                finishStocks[draggingCard->getLastStock() - numOfStocks].addCard(draggingCard);
            }
            else if (draggingCard->getLastStock() == numOfStocks + 4) {
                draggingCard->setIsOnWithdraw(true);
                draggingCard->renderTexture();
                withdrawStock.addCard(draggingCard);
            }
            else {
                showingStock.addCard(draggingCard);
            }
            draggingCard->getShape().setPosition(draggingCard->getPositionOnStockX(), draggingCard->getPositionOnStockY());
            if (isDraggingCards) {
                int j = 0;
                for (Card* c : draggingCards) {
                    stocks[draggingCard->getLastStock()].addCard(c);
                    c->getShape().setPosition(c->getPositionOnStockX(), c->getPositionOnStockY());
                }
                draggingCards.clear();
            }
            draggingCard = nullptr;
        }
            
        setDraggingState(false);
        setDraggingStateCards(false);
        
        //Renderowanie tekstury kazdej karty po akcji
        for (int i = 0; i < numOfStocks; i++) {
            std::vector<Card*>& cards = stocks[i].getCards();
            
            if (!cards.empty()) {
                stocks[i].getTopCardPtr()->setIsOnWithdraw(false);
                for (Card* c : cards) {
                    c->renderTexture();
                }
            }
        }
        //Sprawdzenie czy gra jest wygrana po akcji
        bool checkIfAnyCardIsBehind = false;
        for (int i = 0; i < numOfStocks; i++) {
            std::vector<Card*>& cards = stocks[i].getCards();
            
            if (!cards.empty()) {
                if (checkIfAnyCardIsBehind) {
                    break;
                }
                for (Card* c : cards) {
                    if (c->getIsOnWithdraw()) {
                        checkIfAnyCardIsBehind = true;
                        break;
                    }
                }
            }
            
        }
        if (!checkIfAnyCardIsBehind) {
            isGameWon = true;
        }

    }
}

void Game::update() {
    if (draggingCard) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        draggingCard->getShape().setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
        if (isDraggingCards) {
            for (int i = 0; i < draggingCards.size(); i++){
                draggingCards[i]->getShape().setPosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y) + distY * (i + 1));
            }
        }
    }
}

void Game::render() {
    //window.clear(sf::Color(24, 175, 49));

    // Rysowanie kart ze stosów
    for (int i = 0; i < numOfStocks; ++i) {
        std::vector<Card*>& cards = stocks[i].getCards();
        window.draw(stocks[i].magicRect);
        for (Card* card : cards) {
            window.draw(card->getShape());
        }
    }
    //Rysowanie kart z wygrywajacych stosow
    for (int i = 0; i < 4; i++) {
        if (finishStocks[i].getTopCardPtr()) {
            window.draw(finishStocks[i].getTopCardPtr()->getShape());
        }
        else {
            window.draw(finishStocks[i].magicRect);
        }
    }


    //Rysowanie stosu zabierania
    if (withdrawStock.getTopCardPtr()) {
        window.draw(withdrawStock.getBottomCardPtr()->getShape());
    }
    else {
        window.draw(withdrawStock.magicRect);
    }

    //Rysowanie stosu pokazywania
    if (showingStock.getTopCardPtr()) {
        window.draw(showingStock.getTopCardPtr()->getShape());
    }
    else {
        window.draw(showingStock.magicRect);
    }

    //Rysowanie karty w rece
    if (draggingCard) {
        window.draw(draggingCard->getShape());
    }
    
    //Rysowanie reszty kart w rece
    if (isDraggingCards) {
        for (Card* c : draggingCards) {
            window.draw(c->getShape());
        }
    }

    //window.display();
}
