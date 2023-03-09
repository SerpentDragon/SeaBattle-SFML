#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
    RenderWindow* window;
    RectangleShape button;

    // coordinates and dimensions of the button
    int width;
    int height;  
    int xPos;
    int yPos;

    int pressedCounter;
    
    Color color; // button color
    Color colorOn; // button color when the cursore is over it
    Text text; // button text
    Texture* texture; // button texture

    template <typename T>
    void Swap(T&& obj) noexcept;

    bool OnButton(int x, int y) const;

public:
    Button(RenderWindow *window, const Text& txt, int x, int y, int b_width, int b_height, const Color& color, const Color& colorOn);

    Button(RenderWindow *window, int x, int y, int b_width, int b_height, const Texture* texture);

    Button (const Button& obj);

    Button(Button&& obj) noexcept;

    Button& operator=(const Button& obj);

    Button& operator=(Button&& obj) noexcept;

    ~Button();
    
    int isPressed();

    void setTextColor(const Color& color);

    void setButtonColor(const Color& color);

    int getPressedCounter() const;
};