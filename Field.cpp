#include "Field.h"
#include <iostream>

template<typename T>
void Field::Swap(T&& obj)
{
    window = obj.window;
    field = obj.field;
    x = obj.x;
    y = obj.y;
    size = obj.size;
    data = obj.data;
}

Field::Field(const RenderWindow* window, const int& xPos, const int& yPos)
{
    this->window = const_cast<RenderWindow*>(window);

    x = xPos;
    y = yPos;
    this->size = fieldSize;
    data = 0;

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

void Field::setData(const int& data)
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

void Field::setCurrentColor(const int& valid)
{
    if (valid == -1) setWrongColor();
    else if (valid == 0) setNeutralColor();
    else if (valid == 1) setCorrectColor();
}