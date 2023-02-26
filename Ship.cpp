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
    isPlaced = obj.isPlaced;
    rotateCounter = obj.rotateCounter;
    shipTexture = obj.shipTexture;
    shipRect = obj.shipRect;
    shipRect.setTexture(shipTexture);
    XPos = obj.XPos;
    YPos = obj.YPos;
}

bool Ship::checkCells(const std::vector<Field>& leftField, int currIndex) const
{
    int step = width > height ? 10 : 1;
    for(size_t i = 0; i < deckNumber; i++, currIndex += step) // check if all the cells from the range are free
    {
        if (leftField[currIndex].getData()) return false;
    }
    return true;
}

void Ship::locateShip(std::vector<Field>& leftField, const int& data)
{
    int shipPlace, aroundShip;
    int xIndex, yIndex;
    int i, j, step;
    
    if (!data) 
    {
        shipPlace = aroundShip = 0; // id not data we should free cells
        xIndex = isPlaced / 10;
        yIndex = isPlaced % 10;
        isPlaced = -1;
    }
    else 
    {
        shipPlace = 1; aroundShip = 2; // otherwise we should mark them as taken
        xIndex = (XPos - xCoord) / fieldSize;
        yIndex = (YPos - yCoord) / fieldSize;
        isPlaced = xIndex * 10 + yIndex;
    }

    int leftXBorder = xIndex - 1, rightXBorder = width > height ? xIndex + deckNumber : xIndex + 1; // calculate positions of the left upper and lower right corners of 
    int upYBorder = yIndex - 1, downYBorder = width > height ? yIndex + 1 : yIndex + deckNumber; // the area around the ship

    if (leftXBorder < 0) leftXBorder++; // correct borders if they are out of fields (it means that coordinates are less then 0 or greater then 9)
    else if (rightXBorder > 9) rightXBorder--;
    if (upYBorder < 0) upYBorder++;
    else if (downYBorder > 9) downYBorder--;

    for(i = leftXBorder; i <= rightXBorder; i++) // mark area around the ship as taken
    {
        for(j = upYBorder; j <= downYBorder; j++) leftField[i * 10 + j].setData(aroundShip);
    }

    step = width > height ? 10 : 1;
    for(i = xIndex * 10 + yIndex, j = 0; j < deckNumber; j++, i += step) leftField[i].setData(shipPlace); // mark cells as taken for a ship
}

int Ship::calculatePosition() const
{
    int xIndex = (XPos - xCoord) / fieldSize; // number of the row where the ship is located
    int yIndex = (YPos - yCoord) / fieldSize; // number of the column where the ship is located
    int xOffset = (XPos - xCoord) % fieldSize; // x- and y-offset relative to the upper left corner of the cell where the upper left corner of the ship is located 
    int yOffset = (YPos - yCoord) % fieldSize; 

    if (xOffset > fieldSize - xOffset) xIndex++;
    if (yOffset > fieldSize - yOffset) yIndex++;

    return xIndex * 10 + yIndex;
}

Ship::Ship(const RenderWindow* window, const int& deck, const int& xPos, const int& yPos)
{
    this->window = const_cast<RenderWindow*>(window);

    deckNumber = deck;
    x = XPos = xPos;
    y = YPos = yPos;
    width = deck * 0.034 * screenWidth;
    height = 0.034 * screenWidth;

    isPlaced = -1;

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
    obj.deckNumber = obj.x = obj.y = obj.width = 0;
    obj.height = obj.XPos = obj.YPos = obj.rotateCounter = 0;
    obj.isPlaced = false;
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
        obj.deckNumber = obj.x = obj.y = obj.width = 0;
        obj.height = obj.XPos = obj.YPos = obj.rotateCounter = 0;
        obj.isPlaced = false;
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

    std::swap(width, height); // checnging ship's width & height

    XPos = x - width + dy; YPos = y - dx; // chenge coordinates of the ship

    shipRect.rotate(90); // rotate it and set new position
    shipRect.setPosition(x - width / 2 + dy, y + height / 2 - dx); 
}

void Ship::setFieldColor(std::vector<Field>& leftField)
{
    static int prevIndex = 0, prevWidth = width, prevHeight = height;
    int step, currIndex = calculatePosition();

    if (isPlaced != -1) locateShip(leftField, 0); // check if the ship is placed on the field

    if (xCoord <= XPos && yCoord <= YPos && xCoord + 10 * fieldSize >= XPos + width && yCoord + 10 * fieldSize >= YPos + height) // check if the ship is inside the user field
    {
        step = prevWidth > prevHeight ? 10 : 1; // calculate orientation of the ship
        for(size_t i = 0; i < deckNumber; i++, prevIndex += step) leftField[prevIndex].setNeutralColor(); // set white color of the cells under the ship
        
        int currColor = checkCells(leftField,currIndex) ? 1 : -1; // if all the cells under the ship are free, we should paint them green, else - red
        step = width > height ? 10 : 1;    
        for(size_t i = 0; i < deckNumber; i++, currIndex += step) leftField[currIndex].setCurrentColor(currColor);
        currIndex -= step * deckNumber;

        prevIndex = currIndex; // remember previous coordunates and orientation of the ship
        prevWidth = width; prevHeight = height;
    }
    else 
    {
        step = prevWidth > prevHeight ? 10 : 1;
        for(size_t i = 0; i < deckNumber; i++, prevIndex += step) leftField[prevIndex].setNeutralColor();
        prevIndex = 0;
    }
}

void Ship::placeShip(std::vector<Field>& leftField)
{
    int step = width > height ? 10 : 1;
    int currIndex = calculatePosition();

    if (xCoord <= XPos && yCoord <= YPos && xCoord + 10 * fieldSize >= XPos + width && yCoord + 10 * fieldSize >= YPos + height) // check if the ship is inside the user field
    {
        for(size_t i = 0; i < deckNumber; i++, currIndex += step) leftField[currIndex].setNeutralColor(); // set white color of the cells under the ship
        currIndex -= step * deckNumber;

        if (checkCells(leftField, currIndex)) // if all the cells under the ship are free
        {
            setPosition(leftField[currIndex].getPosition()); // place ship on the field
            locateShip(leftField, 1); // mark area around the ship on the field as taken
        }
        else resetPosition(leftField); // otherwise set to the ship it's starting position
    }
    else resetPosition(leftField); // otherwise set to the ship it's starting position
}

void Ship::resetPosition(std::vector<Field>& leftField) 
{
    if (isPlaced != -1) // if the ship is placed we should free area around it
    {
        locateShip(leftField, 0);
        isPlaced = -1;
    }
    
    int tmpCounter = 4 - rotateCounter;
    while(tmpCounter--) rotateShip(x, y); // rotate ship until it takes it's starting position
    setPosition(x, y);
}

const Vector2f Ship::getPosition() const { return Vector2f(XPos, YPos); }

const bool Ship::getIsPlaced() const { return isPlaced == -1; }

const int Ship::getWidth() const { return width; }

const int Ship::getHeight() const { return height; }

void Ship::setPosition(const Vector2f& vec) 
{
    XPos = vec.x; YPos = vec.y;
    shipRect.setPosition(XPos + width / 2, YPos + height / 2);
}

void Ship::setPosition(const int& xPos, const int& yPos) 
{
    XPos = xPos; YPos = yPos;
    shipRect.setPosition(xPos + width / 2, yPos + height / 2);
}
