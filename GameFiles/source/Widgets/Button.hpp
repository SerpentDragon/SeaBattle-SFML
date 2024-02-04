#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
public:

    Button() = default;

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

    void resetPressedCounter();

private:

    Button(RenderWindow*, int, int, int, int);

    void swap(const Button&) noexcept;

    bool onButton(int, int) const;

private:

    RenderWindow* window_;
    RectangleShape button_;

    // coordinates and dimensions of the button
    int width_;
    int height_;  
    int xPos_;
    int yPos_;

    int pressedCounter_;
    
    Color color_; // button color
    Color colorOn_; // button color when the cursore is over it
    Text text_; // button text
    Texture* texture_; // button texture
    Texture* textureOn_;
};