#include "Ship.h"

// RenderWindow* window;

// int deckNumber;
// int x;
// int y;

// Texture shipTexture;
// RectangleShape shipRect;

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

    shipTexture.loadFromFile("images/ships/" + std::to_string(deck) + "deck.jpg");
    shipRect.setSize(Vector2f(deck * 0.034 * screenWidth, 0.034 * screenWidth));
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

void Ship::drawShip() const
{
    window->draw(shipRect);
}