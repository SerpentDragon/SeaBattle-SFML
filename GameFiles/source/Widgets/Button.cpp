#include "Button.hpp"

void Button::swap(const Button& other) noexcept
{
    window_ = other.window_;
    button_ = other.button_;
    width_ = other.width_;
    height_ = other.height_;
    xPos_ = other.xPos_;
    yPos_ = other.yPos_;
    pressedCounter_ = other.pressedCounter_;
    color_ = other.color_;
    colorOn_ = other.colorOn_;
    text_ = other.text_;
    texture_ = other.texture_ ? new Texture(*other.texture_) : nullptr;
    textureOn_ = other.textureOn_ ? new Texture(*other.textureOn_) : nullptr;
}

// check if the cursor is above the button
bool Button::onButton(int x, int y) const
{
    return xPos_ <= x && x <= xPos_ + width_ 
        && yPos_ <= y && y <= yPos_ + height_;
}

Button::Button(RenderWindow* window, int x, int y, int width, int height)
    : window_(window), xPos_(x), yPos_(y), width_(width), height_(height),
    pressedCounter_(0), button_((Vector2f(width, height)))
{
    button_.setPosition(x, y);
}

Button::Button(RenderWindow *window, const Text& text, 
    int x, int y, int width, int height, 
    const Color& color, const Color& colorOn)
    : Button(window, x, y, width, height)
{
    color_ = color;
    colorOn_ = colorOn;
    text_ = text;
    texture_ = textureOn_ = nullptr;

    button_.setFillColor(color);

    auto textBounds = text_.getGlobalBounds();
    text_.setPosition(x + (width_ - textBounds.width) / 2 , 
        y + (height_ - textBounds.height) / 2 - 7);
}

Button::Button(RenderWindow* window, const Text& text, 
    int x, int y, int width, int height, 
    const Texture* texture, const Texture* textureOn, 
    const Color& textColor, const Color& textColorOn)
    : Button(window, x, y, width, height)
{
    color_ = textColor;
    colorOn_ = textColorOn;

    texture_ = texture ? new Texture(*texture) : new Texture();
    textureOn_ = textureOn ? new Texture(*textureOn) : new Texture();

    text_ = text;
    auto textBounds = text_.getLocalBounds();
    text_.setPosition(x + (width_ - textBounds.width) / 2 , 
        y + (height_ - textBounds.height) / 2 - 7);   
}

Button::Button(RenderWindow *window, int x, int y, 
    int width, int height, const Texture* texture)
    : Button(window, x, y, width, height)
{
    color_ = colorOn_ = Color(0, 0, 0, 255);
    text_ = Text();

    texture_ = texture ? new Texture(*texture) : new Texture();

    button_.setTexture(texture_); 
}

Button::Button(RenderWindow *window, const Text& text, 
    int x, int y, int width, int height, 
    const Texture* texture)
    : Button(window, x, y, width, height)
{
    color_ = colorOn_ = Color(0, 0, 0, 255);
    text_ = text;
    
    texture_ = texture ? new Texture(*texture) : new Texture();

    button_.setTexture(texture_); 
}

Button::Button(const Button& other)
{
    swap(other);
}

Button::Button(Button&& other) noexcept
{
    swap(other);

    other.window_ = nullptr;
    other.width_ = other.height_ = other.xPos_ = 
        other.yPos_ = other.pressedCounter_ = 0;
    other.texture_ = other.textureOn_ = nullptr;
}

Button& Button::operator=(const Button& other)
{
    if (this != &other)
    {
        swap(other);
    }
    return *this;
}

Button& Button::operator=(Button&& other) noexcept
{
    if (this != &other)
    {
        swap(other);

        other.window_ = nullptr;
        other.width_ = other.height_ = other.xPos_ = 
            other.yPos_ = other.pressedCounter_ = 0;
        other.texture_ = other.textureOn_ = nullptr;
    }
    return *this;
}

Button::~Button()
{
    window_ = nullptr;
    if (texture_) delete texture_;
    if (textureOn_) delete textureOn_;
}

void Button::drawButton() const
{
    window_->draw(button_);
    window_->draw(text_);
}

int Button::isPressed()
{
    int x = Mouse::getPosition(*window_).x;
    int y = Mouse::getPosition(*window_).y;

    window_->draw(button_);
    window_->draw(text_);
    
    if (onButton(x, y))
    {
        if (!texture_) button_.setFillColor(colorOn_);
        else 
        {
            text_.setFillColor(colorOn_);
            button_.setTexture(textureOn_);
        }
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            while(true) // while left mouse button is held
            {
                if (!Mouse::isButtonPressed(Mouse::Left)) 
                    break;
            }
            if (onButton(Mouse::getPosition(*window_).x, Mouse::getPosition(*window_).y)) 
                return ++pressedCounter_; // the button was pressed
        }
    }
    else 
    {
        if (!texture_) button_.setFillColor(color_); // else if the cursor is not above the button and it has no texture paint it "color" color
        else if (texture_)
        {
            text_.setFillColor(color_);
            button_.setTexture(texture_);
        }
    }

    return 0;
}

void Button::setTextColor(const Color& color)
{
    text_.setFillColor(color);
}

void Button::setButtonColor(const Color& color)
{
    color_ = color;
    button_.setFillColor(color);
}

void Button::setText(const wchar_t* string)
{
    text_.setString(string);

    auto textBounds = text_.getLocalBounds();
    text_.setPosition(xPos_ + (width_ - textBounds.width) / 2 , 
        yPos_ + (height_ - textBounds.height) / 2 - 7); 
}

int Button::getPressedCounter() const { return pressedCounter_; }

void Button::resetPressedCounter() { pressedCounter_ = 0; }
