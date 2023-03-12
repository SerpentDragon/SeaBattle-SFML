#include "Field.h"
#include <iostream>

/*
data:
0 - the field is free
1 - the field is taken for the ship
2 - the field is free but there is a ship next to it
3 - hit ship
4 - hit field
*/

bool Field::onField(int xPos, int yPos) const
{
    return x <= xPos && xPos <= x + size && y <= yPos && yPos <= y + size; // check if the cursor is above the field
}

template<typename T>
void Field::Swap(T&& obj)
{
    window = obj.window;
    field = obj.field;
    x = obj.x;
    y = obj.y;
    size = obj.size;
    data = obj.data;
    hit = obj.hit;
    miss = obj.miss;
}

Field::Field(const RenderWindow* window, int xPos, int yPos)
{
    this->window = const_cast<RenderWindow*>(window);

    x = xPos;
    y = yPos;
    this->size = fieldSize;
    data = 0;

    // loading textures depending on what area the field is located
    hit = xPos < xCoord + 12 * fieldSize ? Texture() : playerHitTexture; 
    miss = xPos < xCoord + 12 * fieldSize ? computerMissedTexture : playerMissedTexture;

    field = RectangleShape(Vector2f(size, size));

    field.setOutlineThickness(0.0019 * screenHeight);
    field.setOutlineColor(Color::Black);
    field.setFillColor(Color::White);
    field.setPosition(x, y);
}

Field::Field(const Field& obj)
{
    Swap(obj);
}

Field::Field(Field&& obj)
{
    Swap(obj);

    obj.window = nullptr;
    obj.x = obj.y = obj.size = obj.data = 0;
}

Field& Field::operator=(const Field& obj)
{
    if (this != &obj)
    {
        Swap(obj);
    }
    return *this;
}

Field& Field::operator=(Field&& obj)
{
    if (this != &obj)
    {
        Swap(obj);

        obj.window = nullptr;
        obj.x = obj.y = obj.size = obj.data = 0;
    }
    return *this;
}

Field::~Field()
{
    window = nullptr;
}

void Field::drawField() const
{
    window->draw(field);
}

const Vector2f Field::getPosition() const { return Vector2f(x, y); }

const int Field::getData() const { return data; }

void Field::setData(int data)
{
    this->data = data;
}

void Field::setCorrectColor() 
{ 
    field.setFillColor(Color::Green); 
}

void Field::setWrongColor() 
{ 
    field.setFillColor(Color::Red); 
}

void Field::setNeutralColor() 
{ 
    field.setFillColor(Color::White); 
}

void Field::setCurrentColor(int valid)
{
    if (valid == -1) setWrongColor();
    else if (valid == 0) setNeutralColor();
    else if (valid == 1) setCorrectColor();
}

void Field::displayHitTexture() 
{
    field.setTexture(&hit);
}

void Field::displayMissTexture() 
{ 
    field.setTexture(&miss); 
}

bool Field::isChosen() const
{
    int x = Mouse::getPosition(*window).x;
    int y = Mouse::getPosition(*window).y;
    
    if (onField(x, y)) // if the cursor is above the field
    {
        if (Mouse::isButtonPressed(Mouse::Left))  // if left mouse button was pressed
        {
            while(true) // while left mouse button is held
            {
                if (!Mouse::isButtonPressed(Mouse::Left)) break; // if left mouse button was released 
            }
            if (onField(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)) return true; // the field was chosen
        }
    }

    return false;
}
