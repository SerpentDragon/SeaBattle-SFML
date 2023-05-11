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
    Texture *textureOn;

    template <typename T>
    void Swap(T&&) noexcept;

    bool OnButton(int, int) const;

public:
    Button(RenderWindow*, const Text&, int, int, int, int, const Color&, const Color&);

    Button(RenderWindow*, const Text&, int, int, int, int, const Texture*, const Texture*, const Color&, const Color&);

    Button(RenderWindow *, const Text&, int, int, int, int, const Texture*);

    Button(RenderWindow*, int, int, int, int, const Texture*);

    Button (const Button&);

    Button(Button&&) noexcept;

    Button& operator=(const Button&);

    Button& operator=(Button&&) noexcept;

    ~Button();
    
    void drawButton() const;
    
    int isPressed();

    void setTextColor(const Color&);

    void setButtonColor(const Color&);

    void setText(const wchar_t*);

    int getPressedCounter() const;
};