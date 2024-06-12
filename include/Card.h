#ifndef CARD_H
#define CARD_H

#include "API.h"
#include <SFML/Graphics.hpp>
#include <string>

class EXPORT_API Game; // Forward declaration

class EXPORT_API Card 
{
public:
    Card(float width, float height, int numOfCard, int colOfCard);

    sf::RectangleShape& getShape();

    const sf::Texture& getBackTexture() const;
    const sf::Texture& getFrontTexture() const;

    void setPositionOnStock(float x, float y);
    float getPositionOnStockX();
    float getPositionOnStockY();

    void setLastStock(int last);
    int getLastStock();

    int getNumberOfCard();
    int getColorOfCard();


    void renderTexture();
    void setIsOnWithdraw(bool value);
    bool getIsOnWithdraw();

private:
    sf::RectangleShape cardShape;
    sf::Texture cardBackTexture;
    sf::Texture cardFrontTexture;
    std::string cardFrontString;
    bool isOnWithdraw;

    //number karty AS-1 2-10 Jopek 11 Dama 12 Krol 13
    int numberOfCard;
    std::string numberOfCardString;
    //Kolor karty 0 - pik(czarne wino, lisc debu); 1 - kier(czerwone serce); 2 - trefl(czarny zoladz); 3 - karo(czerwony diament)
    int colorOfCard;
    std::string colorOfCardString;

    //tylko do odwracania karty do wywalenia
    bool isMouseOverFlag;

    //isOnTop tylko do odwracania karty do wywalenia
    bool isOnTop;
    //do odwaracania do wywalenia
    bool isDragging;

    int lastStock;
    float positionOnStockX;
    float positionOnStockY;
};

#endif // CARD_H
