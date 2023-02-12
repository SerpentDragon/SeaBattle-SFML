#include "Ship.h"
#include <iostream>

template <typename T>
void Ship::Swap(T&& obj) noexcept
{
    window = obj.window;
    deckNumber = obj.deckNumber;
    x = obj.x;
    y = obj.y;
    width = obj.width;
    height = obj.height;
    shipTexture = obj.shipTexture;
    shipRect = obj.shipRect;
    shipRect.setTexture(shipTexture);
    XPos = obj.XPos;
    YPos = obj.YPos;
}

Ship::Ship(const RenderWindow* window, const int& deck, const int& xPos, const int& yPos)
{
    this->window = const_cast<RenderWindow*>(window);

    deckNumber = deck;
    x = XPos = xPos;
    y = YPos = yPos;
    width = deck * 0.034 * screenWidth;
    height = 0.034 * screenWidth;

    shipTexture.loadFromFile("images/ships/" + std::to_string(deck) + "deck.png");

    shipRect.setTexture(shipTexture);
    shipRect.setScale(static_cast<double>(width) / shipTexture.getSize().x, static_cast<double>(height) / shipTexture.getSize().y);
    shipRect.setOrigin(shipRect.getLocalBounds().width / 2, shipRect.getLocalBounds().height / 2);
    shipRect.setPosition(x + width / 2, y + height / 2);
}

Ship::Ship(const Ship& obj)
{
    Swap(obj);
}

Ship::Ship(Ship&& obj) noexcept 
{
    Swap(obj);    

    obj.window = nullptr;
    obj.deckNumber = obj.x = obj.y = obj.width = obj.height = obj.XPos = obj.YPos = 0;
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
        obj.deckNumber = obj.x = obj.y = obj.width = obj.height = obj.XPos = obj.YPos = 0;
    }
    return *this;
}

Ship::~Ship()
{
    window = nullptr;
}

bool Ship::onShip(const int& xPos, const int& yPos) const
{
    return shipRect.getGlobalBounds().contains(xPos, yPos);
}

void Ship::drawShip() const
{
    window->draw(shipRect);
}

void Ship::setPos(const int& x, const int& y) 
{
    XPos = x; YPos = y;
    shipRect.setPosition(x + width / 2, y + height / 2);
}

const Vector2f Ship::getPos() const { return Vector2f(XPos, YPos); }

void Ship::rotateShip(const int& x, const int& y)
{
    int dx = x - XPos;
    int dy = y - YPos;

    std::swap(width, height);

    XPos = x - width + dy; YPos = y - dx;

    shipRect.rotate(90);
    shipRect.setPosition(x - width / 2 + dy, y + height / 2 - dx);
}
