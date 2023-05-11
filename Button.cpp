#include "Button.h"

#include <iostream>

template <typename T>
void Button::Swap(T&& obj) noexcept
{
    window = obj.window;
    button = obj.button;
    width = obj.width;
    height = obj.height;
    xPos = obj.xPos;
    yPos = obj.yPos;
    pressedCounter = obj.pressedCounter;
    color = obj.color;
    colorOn = obj.colorOn;
    text = obj.text;
    texture = obj.texture ? new Texture(*obj.texture) : nullptr;
    textureOn = obj.textureOn ? new Texture(*obj.textureOn) : nullptr;
}

bool Button::OnButton(int x, int y) const
{
    return xPos <= x && x <= xPos + width && yPos <= y && y <= yPos + height; // check if the cursor is above the button
}

Button::Button(RenderWindow *window, const Text& text, int x, int y, int b_width, int b_height, const Color& color, const Color& colorOn)
{
    this->window = window;
    xPos = x;
    yPos = y;
    width = b_width;
    height = b_height;
    pressedCounter = 0;
    this->color = color;
    this->colorOn = colorOn;
    this->text = text;
    texture = textureOn = nullptr;

    button = RectangleShape(Vector2f(width, height));
    button.setPosition(x, y);
    button.setFillColor(color);

    this->text.setPosition(x + (width - this->text.getLocalBounds().width) / 2 , y + (height - this->text.getLocalBounds().height) / 2 - 7);   
}

Button::Button(RenderWindow* window, const Text& text, int x, int y, int b_width, int b_height, const Texture* texture, const Texture* textureOn, const Color& textColor, const Color& textColorOn)
{
    this->window = window;
    xPos = x;
    yPos = y;
    width = b_width;
    height = b_height;
    pressedCounter = 0;
    color = textColor;
    colorOn = textColorOn;
    this->text = text;
    this->texture = texture ? new Texture(*texture) : new Texture();
    this->textureOn = textureOn ? new Texture(*textureOn) : new Texture();

    button = RectangleShape(Vector2f(width, height));
    button.setPosition(x, y);

    this->text.setPosition(x + (width - this->text.getLocalBounds().width) / 2 , y + (height - this->text.getLocalBounds().height) / 2 - 7);   
}

Button::Button(RenderWindow *window, int x, int y, int b_width, int b_height, const Texture* texture)
{
    this->window = window;
    xPos = x;
    yPos = y;
    width = b_width;
    height = b_height;
    pressedCounter = 0;
    color = colorOn = Color(0, 0, 0, 255);
    text = Text();
    this->texture = texture ? new Texture(*texture) : new Texture();

    button = RectangleShape(Vector2f(width, height));
    button.setPosition(x, y);
    button.setTexture(this->texture); 
}

Button::Button(RenderWindow *window, const Text& text, int x, int y, int b_width, int b_height, const Texture* texture)
{
    this->window = window;
    xPos = x;
    yPos = y;
    width = b_width;
    height = b_height;
    pressedCounter = 0;
    color = colorOn = Color(0, 0, 0, 255);
    this->text = text;
    this->texture = texture ? new Texture(*texture) : new Texture();

    button = RectangleShape(Vector2f(width, height));
    button.setPosition(x, y);
    button.setTexture(this->texture); 
}

Button::Button (const Button& obj)
{
    Swap(obj);
}

Button::Button(Button&& obj) noexcept
{
    Swap(obj);

    obj.window = nullptr;
    obj.width = obj.height = obj.xPos = obj.yPos = obj.pressedCounter = 0;
    obj.texture = obj.textureOn = nullptr;
}

Button& Button::operator=(const Button& obj)
{
    if (this != &obj)
    {
        Swap(obj);
    }
    return *this;
}

Button& Button::operator=(Button&& obj) noexcept
{
    if (this != &obj)
    {
        Swap(obj);

        obj.window = nullptr;
        obj.width = obj.height = obj.xPos = obj.yPos = obj.pressedCounter = 0;
        obj.texture = obj.textureOn = nullptr;
    }
    return *this;
}

Button::~Button()
{
    window = nullptr;
    if (texture) delete texture;
    if (textureOn) delete textureOn;
}

void Button::drawButton() const
{
    window->draw(button);
    window->draw(text);
}

int Button::isPressed()
{
    int x = Mouse::getPosition(*window).x;
    int y = Mouse::getPosition(*window).y;

    window->draw(button);
    window->draw(text);
    
    if (OnButton(x, y)) // if the cursor is above the button
    {
        if (!texture) button.setFillColor(colorOn); // if button has no texture paint it "colorOn" color
        else 
        {
            text.setFillColor(colorOn);
            button.setTexture(textureOn);
        }
        if (Mouse::isButtonPressed(Mouse::Left))  // if left mouse button was pressed
        {
            while(true) // while left mouse button is held
            {
                if (!Mouse::isButtonPressed(Mouse::Left)) break; // if left mouse button was released 
            }
            if (OnButton(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y)) return ++pressedCounter; // the button was pressed
        }
    }
    else 
    {
        if (!texture) button.setFillColor(color); // else if the cursor is not above the button and it has no texture paint it "color" color
        else if (texture)
        {
            text.setFillColor(color);
            button.setTexture(texture);
        }
    }

    return 0;
}

void Button::setTextColor(const Color& color)
{
    text.setFillColor(color);
}

void Button::setButtonColor(const Color& color)
{
    this->color = color;
    button.setFillColor(color);
}

void Button::setText(const wchar_t* string)
{
    text.setString(string);
    text.setPosition(xPos + (width - text.getLocalBounds().width) / 2 , yPos + (height - text.getLocalBounds().height) / 2 - 7); 
}

int Button::getPressedCounter() const { return pressedCounter; }
