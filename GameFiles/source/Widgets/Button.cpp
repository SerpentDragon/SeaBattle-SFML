#include "Button.hpp"

// check if the cursor is above the button
bool Button::onButton(int x, int y) const
{
    return xPos_ <= x && x <= xPos_ + width_ 
        && yPos_ <= y && y <= yPos_ + height_;
}

Button::Button(std::shared_ptr<RenderWindow> window, int x, int y, 
    int width, int height) : window_(window), xPos_(x), yPos_(y), 
    width_(width), height_(height), button_((Vector2f(width, height)))
{
    button_.setPosition(x, y);
}

Button::Button(std::shared_ptr<RenderWindow>window, const Text& text, 
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

Button::Button(std::shared_ptr<RenderWindow> window, const Text& text, 
    int x, int y, int width, int height, std::shared_ptr<Texture> texture, 
    std::shared_ptr<Texture> textureOn, const Color& textColor, 
    const Color& textColorOn) : Button(window, x, y, width, height)
{
    color_ = textColor;
    colorOn_ = textColorOn;

    texture_ = texture ? 
        texture : std::shared_ptr<Texture>(new Texture());
    textureOn_ = textureOn 
        ? textureOn : std::shared_ptr<Texture>(new Texture());

    text_ = text;
    auto textBounds = text_.getLocalBounds();
    text_.setPosition(x + (width_ - textBounds.width) / 2 , 
        y + (height_ - textBounds.height) / 2 - 7);   
}

Button::Button(std::shared_ptr<RenderWindow>window, int x, int y, 
    int width, int height, std::shared_ptr<Texture> texture)
    : Button(window, x, y, width, height)
{
    color_ = colorOn_ = Color(0, 0, 0, 255);
    text_ = Text();

    texture_ = texture ? 
        texture : std::shared_ptr<Texture>(new Texture());

    button_.setTexture(&(*texture_));
}

Button::Button(std::shared_ptr<RenderWindow>window, const Text& text, 
    int x, int y, int width, int height, 
    std::shared_ptr<Texture> texture)
    : Button(window, x, y, width, height)
{
    color_ = colorOn_ = Color(0, 0, 0, 255);
    text_ = text;
    
    texture_ = texture ? 
        texture : std::shared_ptr<Texture>(new Texture());

    button_.setTexture(&(*texture_));
}

void Button::drawButton() const
{
    window_->draw(button_);
    window_->draw(text_);
}

bool Button::isPressed()
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
            button_.setTexture(&(*textureOn_));
        }
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            while(true) // while left mouse button is held
            {
                if (!Mouse::isButtonPressed(Mouse::Left)) 
                    break;
            }
            if (onButton(Mouse::getPosition(*window_).x, Mouse::getPosition(*window_).y)) 
                return true; // the button was pressed
        }
    }
    else 
    {
        if (!texture_) button_.setFillColor(color_);
        else if (texture_)
        {
            text_.setFillColor(color_);
            button_.setTexture(&(*texture_));
        }
    }

    return false;
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

void Button::setText(const std::wstring& string)
{
    text_.setString(string);

    auto textBounds = text_.getLocalBounds();
    text_.setPosition(xPos_ + (width_ - textBounds.width) / 2 , 
        yPos_ + (height_ - textBounds.height) / 2 - 7); 
}

void Button::setTextFontSize(int fontSize)
{
    text_.setCharacterSize(fontSize);

    auto textBounds = text_.getLocalBounds();
    text_.setPosition(xPos_ + (width_ - textBounds.width) / 2 , 
        yPos_ + (height_ - textBounds.height) / 2); 
}
