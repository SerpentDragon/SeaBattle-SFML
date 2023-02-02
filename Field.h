#include "settings.h"

class Field
{
    RenderWindow* window;
    
    RectangleShape field;

    int x;
    int y;
    int size;

    static int a;

public:
    Field(const RenderWindow* window, const int& xPos, const int& yPos, const int& size)
    {
        this->window = const_cast<RenderWindow*>(window);

        x = xPos;
        y = yPos;
        this->size = size;

        field = RectangleShape(Vector2f(size, size));
        field.setOutlineThickness(1);
        field.setPosition(x, y);
        field.setFillColor(Color::White);
    }

    void drawField() const
    {
        window->draw(field);
    }

    void setGoalImage() 
    {
        
    }

    void setMissImage() const
    {

    }

};