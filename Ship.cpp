#include "Ship.h"
#include <iostream>

static int rotateCounter = 0;

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

void Ship::drawShip() const
{
    window->draw(shipRect);
}

bool Ship::onShip(const int& xPos, const int& yPos) const
{
    return shipRect.getGlobalBounds().contains(xPos, yPos);
}

void Ship::rotateShip(const int& x, const int& y)
{
    ++rotateCounter %= 4;

    int dx = x - XPos;
    int dy = y - YPos;

    std::swap(width, height);

    XPos = x - width + dy; YPos = y - dx;

    shipRect.rotate(90);
    shipRect.setPosition(x - width / 2 + dy, y + height / 2 - dx);
}

void Ship::setFieldPosition(std::vector<Field>& leftField, const int& isReleased)
{
    int Xindex = (XPos - xCoord) / fieldSize; // number of the row where the ship is located
    int Yindex = (YPos - yCoord) / fieldSize; // number of the column where the ship is located
    int Xoffset = (XPos - xCoord) % fieldSize; // x- and y-offset relative to the upper left corner of the cell where the upper left corner of the ship is located 
    int Yoffset = (YPos - yCoord) % fieldSize; 

    if(Xoffset > fieldSize - Xoffset) Xindex++;
    if(Yoffset > fieldSize - Yoffset) Yindex++;

    static int prevIndex = 0, prevWidth = width, prevHeight = height;
    int step, currIndex = Xindex * 10 + Yindex;

    if (isReleased)
    {
        if (xCoord <= XPos && yCoord <= YPos && xCoord + 10 * fieldSize >= XPos + width && yCoord + 10 * fieldSize >= YPos + height)
        {
            setPos(xCoord + Xindex * fieldSize, yCoord + Yindex * fieldSize);
            step = width > height ? 10 : 1;
            for(size_t i = 0; i < deckNumber; i++, currIndex += step) leftField[currIndex].setNeutralColor();
        }
        else 
        {
            int tmpCounter = 4 - rotateCounter;
            while(tmpCounter--) rotateShip(x, y);
            setPos(x, y);
        }
        return;
    }

    if (xCoord <= XPos && yCoord <= YPos && xCoord + 10 * fieldSize >= XPos + width && yCoord + 10 * fieldSize >= YPos + height)
    {
        step = prevWidth > prevHeight ? 10 : 1;
        for(size_t i = 0; i < deckNumber; i++, prevIndex += step) leftField[prevIndex].setNeutralColor();

        step = width > height ? 10 : 1;
        for(size_t i = 0; i < deckNumber; i++, currIndex += step) leftField[currIndex].setCorrectColor();
        currIndex -= step * deckNumber;

        prevIndex = currIndex;
        prevWidth = width; prevHeight = height;
    }
    else 
    {
        step = prevWidth > prevHeight ? 10 : 1;
        for(size_t i = 0; i < deckNumber; i++, prevIndex += step) leftField[prevIndex].setNeutralColor();
        prevIndex = 0;
    }
}


const Vector2f Ship::getPos() const { return Vector2f(XPos, YPos); }

const int Ship::getWidth() const { return width; }

const int Ship::getHeight() const { return height; }

void Ship::setPos(const int& xPos, const int& yPos) 
{
    XPos = xPos; YPos = yPos;
    shipRect.setPosition(xPos + width / 2, yPos + height / 2);
}
