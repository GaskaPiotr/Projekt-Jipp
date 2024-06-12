#include "Stock.h"
#include <iostream>

Stock::~Stock() {
    for (Card* card : cards) {
        delete card; // Usuwanie dynamicznie alokowanych kart
    }
}

void Stock::addCard(Card* card) {
    cards.push_back(card);
}


void Stock::removeTopCard() {
    if (!cards.empty()) {
        cards.pop_back();
    }
}

void Stock::removeBottomCard() {
    if (!cards.empty()) {
        cards.erase(cards.begin());
    }
}

void Stock::removeCardsBelow(int howMany) {
    for (int i = 0; i < howMany; i++) {
        if (!cards.empty()) {
            cards.pop_back();
        }
    }
}

Card* Stock::getTopCardPtr() {
    if (!cards.empty()) {
        return cards.back();
    }
    return nullptr;
}

Card* Stock::getBottomCardPtr() {
    if (!cards.empty()) {
        return cards.front();
    }
    return nullptr;
}

std::vector<Card*>& Stock::getCards() {
    return cards;
}

int Stock::getCardsSize() {
    return cards.size();
}

std::vector<Card*>& Stock::getCardsBelow(Card* card) {
    int i = 0;
    cardsToReturn.clear();
    while (card != cards[i]) {
        i++;
    }
    i++;
    while (i < getCardsSize()) {
        cardsToReturn.push_back(cards[i]);
        i++;
    }
    return cardsToReturn;
}


bool Stock::isMouseOverLastCard(const sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect bounds;
    if (!cards.empty()) {
        bounds = cards.back()->getShape().getGlobalBounds();
    }
    else {
        bounds = magicRect.getGlobalBounds();
    }
    return bounds.contains(static_cast<sf::Vector2f>(mousePos));
}

Card* Stock::whichCardIsMouseOver(const sf::RenderWindow& window) const {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::FloatRect bounds;
    Card* cardOnMouse = nullptr;
    for (Card* c : cards) {
        bounds = c->getShape().getGlobalBounds();
        if (bounds.contains(static_cast<sf::Vector2f>(mousePos))) {
            cardOnMouse = c;
        }
    }
    return cardOnMouse;
}

bool Stock::contains(Card* card) const {
    for (Card* c : cards) {
        if (c == card) {
            return true;
        }
    }
    return false;
}
float Stock::getPositionX() {
    return positionX;
}
float Stock::getPositionY() {
    return positionY;
}
void Stock::setPosition(float posX, float posY) {
    positionX = posX;
    positionY = posY;
}
