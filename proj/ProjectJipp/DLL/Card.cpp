#include "Card.h"
#include "Game.h"
#include <iostream>

Card::Card(float width, float height, int numOfCard, int colOfCard)
    : cardShape(sf::Vector2f(width, height)), isMouseOverFlag(false), isOnTop(true), isDragging(false), positionOnStockX(0), positionOnStockY(0), isOnWithdraw(true) {
    cardShape.setOrigin(width / 2.0f, height / 2.0f);
    numberOfCard = numOfCard;
    numberOfCardString = std::to_string(numberOfCard);
    
    colorOfCard = colOfCard;
    //Kolor karty 0 - pik(czarne wino, lisc debu); 1 - kier(czerwone serce); 2 - trefl(czarny zoladz); 3 - karo(czerwony diament)
    if (colorOfCard == 0) {
        colorOfCardString = "pik";
    }
    else if (colorOfCard == 1) {
        colorOfCardString = "kier";
    }
    else if (colorOfCard == 2) {
        colorOfCardString = "trefl";
    }
    else if (colorOfCard == 3) {
        colorOfCardString = "karo";
    }
    else {
        std::cerr << "Bad card color\n";
    }
    cardFrontString = "textures/" + numberOfCardString + " " + colorOfCardString + ".png";
    if (!cardBackTexture.loadFromFile("textures/card back red.png")) {
        std::cerr << "Failed to load back texture\n";
    }
    else {
        cardShape.setTexture(&cardBackTexture);
    }

    if (!cardFrontTexture.loadFromFile(cardFrontString)) {
        std::cerr << "Failed to load front texture\n";
    }
}

sf::RectangleShape& Card::getShape() {
    return cardShape;
}

const sf::Texture& Card::getBackTexture() const {
    return cardBackTexture;
}

const sf::Texture& Card::getFrontTexture() const {
    return cardFrontTexture;
}

void Card::setPositionOnStock(float x, float y) {
    positionOnStockX = x;
    positionOnStockY = y;
    cardShape.setPosition(x, y);
}

float Card::getPositionOnStockX() {
    return positionOnStockX;
}

float Card::getPositionOnStockY() {
    return positionOnStockY;
}

void Card::setLastStock(int last) {
    lastStock = last;
}

int Card::getLastStock() {
    return lastStock;
}


int Card::getNumberOfCard() {
    return numberOfCard;
}
int Card::getColorOfCard() {
    return colorOfCard;
}



void Card::renderTexture() {
    if (isOnWithdraw) {
        cardShape.setTexture(&cardBackTexture);
    }
    else {
        cardShape.setTexture(&cardFrontTexture);
    }
}


void Card::setIsOnWithdraw(bool value) {
    isOnWithdraw = value;
}


bool Card::getIsOnWithdraw() {
    return isOnWithdraw;
}