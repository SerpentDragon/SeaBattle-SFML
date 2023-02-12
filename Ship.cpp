#include "Ship.h"
#include <iostream>

template <typename T>
void Ship::Swap(T&& obj) noexcept
{
    window = obj.window;
    deckNumber = obj.deckNumber;
    x = obj.x;
    y = obj.y;
    shipTexture = obj.shipTexture;
    shipRect = obj.shipRect;
    shipRect.setTexture(&shipTexture);
}

Ship::Ship(const RenderWindow* window, const int& deck, const int& xPos, const int& yPos)
{
    this->window = const_cast<RenderWindow*>(window);

    deckNumber = deck;
    x = xPos;
    y = yPos;

    shipTexture.loadFromFile("images/ships/" + std::to_string(deck) + "deck.png");
    shipRect.setSize(Vector2f(static_cast<int>(deck * 0.034 * screenWidth), static_cast<int>(0.034 * screenWidth)));
    shipRect.setTexture(&shipTexture);
    shipRect.setPosition(x, y);
}

Ship::Ship(const Ship& obj)
{
    Swap(obj);
}

Ship::Ship(Ship&& obj) noexcept
{
    Swap(obj);    

    obj.window = nullptr;
    obj.deckNumber = obj.x = obj.y = 0;
}

Ship& Ship::operator=(const Ship& obj)
{
    if (this != &obj)
    {
        Swap(obj);
    }
    return *this;
}

Ship& Ship::operator=(Ship&& obj) noexcept
{
    if (this != &obj)
    {
        Swap(obj);

        obj.window = nullptr;
        obj.deckNumber = obj.x = obj.y = 0;
    }
    return *this;
}

Ship::~Ship()
{
    window = nullptr;
}

bool Ship::dragAndDrop()
{
    window->draw(shipRect);

    static int prevX = 0; //Mouse::getPosition(*window).x;
    static int prevY = 0; //Mouse::getPosition(*window).y;

    int currX = Mouse::getPosition(*window).x;
    int currY = Mouse::getPosition(*window).y;

    // std::cout << prevX << " " << prevY << std::endl;
    // std::cout << shipRect.getPosition().x << " " << shipRect.getPosition().x + shipRect.getGlobalBounds().width << std::endl;
    // std::cout << shipRect.getPosition().y << " " << shipRect.getPosition().y + shipRect.getGlobalBounds().height << std::endl;
    // std::cout << (shipRect.getPosition().x <= prevX <= shipRect.getPosition().x + shipRect.getGlobalBounds().width) << " " << (shipRect.getPosition().y <= prevY <= shipRect.getPosition().y + shipRect.getGlobalBounds().height)<<std::endl;
    // std::cout << (shipRect.getPosition().y <= prevY <= (shipRect.getPosition().y + shipRect.getGlobalBounds().height)) << std::endl;
    // std::cout << prevX << " " << prevY << std::endl;


    if (shipRect.getPosition().x <= currX  && currX <= shipRect.getPosition().x + shipRect.getGlobalBounds().width && 
        shipRect.getPosition().y <= currY  && currY <= shipRect.getPosition().y + shipRect.getGlobalBounds().height)
    {
        // std::cout << "Yes\n";
        // prevX = currX; prevY = currY;
        // return true;
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            
            if (prevX != currX || prevY != currY)
            {
                // shipRect.setPosition(prevX - shipRect.getPosition().x + currX, prevY - shipRect.getPosition().y + currY);
                shipRect.setPosition(currX, currY);
                prevX = currX; prevY = currY;
            }
        }
    }
    return false;
}