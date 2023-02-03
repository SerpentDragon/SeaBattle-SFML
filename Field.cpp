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
}

Field::Field(const RenderWindow* window, const int& xPos, const int& yPos, const int& size)
{
    this->window = const_cast<RenderWindow*>(window);

    x = xPos;
    y = yPos;
    this->size = size;

    field = RectangleShape(Vector2f(size, size));
    field.setOutlineThickness(2);
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
    obj.x = obj.y = obj.size = 0;
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
        obj.x = obj.y = obj.size = 0;
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