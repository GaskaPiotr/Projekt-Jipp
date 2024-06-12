#ifndef STOCK_H
#define STOCK_H


#include "API.h"
#include <vector>
#include "Card.h"
#include <SFML/Graphics.hpp>

class EXPORT_API Stock {
public:

    ~Stock(); // Destruktor do zarz¹dzania dynamiczn¹ pamiêci¹
    void addCard(Card* card);
    void removeTopCard(); // Nowa metoda do usuwania karty ze stosu
    void removeCardsBelow(int howMany);
    void removeBottomCard();

    Card* getTopCardPtr();
    Card* getBottomCardPtr();
    
    std::vector<Card*>& getCards(); // Metoda do uzyskania dostêpu do wszystkich kart

    int getCardsSize();

    std::vector<Card*>& getCardsBelow(Card* card);

    bool isMouseOverLastCard(const sf::RenderWindow& window) const; // Nowa metoda do sprawdzania, czy myszka jest nad stosem
    Card* whichCardIsMouseOver(const sf::RenderWindow& window) const;
    bool contains(Card* card) const; // Nowa metoda do sprawdzania, czy stos zawiera dan¹ kartê
    float getPositionX();
    float getPositionY();
    void setPosition(float posX, float posY);
    sf::RectangleShape magicRect;
    sf::Texture magicTexture;


private:
    std::vector<Card*> cards;
    std::vector<Card*> cardsToReturn;
    float positionX;
    float positionY;
};

#endif // STOCK_H
