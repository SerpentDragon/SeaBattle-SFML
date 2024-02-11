#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
public:

    Button() = default;

    Button(std::shared_ptr<RenderWindow>, const Text&, int, int, int, int, 
        const Color&, const Color&);

    Button(std::shared_ptr<RenderWindow>, const Text&, int, int, int, int, 
        std::shared_ptr<Texture>, std::shared_ptr<Texture>, const Color&, const Color&);

    Button(std::shared_ptr<RenderWindow>, const Text&, int, int, int, int, 
        std::shared_ptr<Texture>);

    Button(std::shared_ptr<RenderWindow>, int, int, int, int, std::shared_ptr<Texture>);

    Button (const Button&) = default;

    Button(Button&&) noexcept = default;

    Button& operator=(const Button&) = default;

    Button& operator=(Button&&) noexcept = default;

    ~Button() = default;

public:
    
    void drawButton() const;
    
    bool isPressed();

    void setTextColor(const Color&);

    void setButtonColor(const Color&);

    void setText(const std::wstring&);

    void setTextFontSize(int);

private:

    Button(std::shared_ptr<RenderWindow>, int, int, int, int);

    bool onButton(int, int) const;

private:

    std::shared_ptr<RenderWindow> window_;
    RectangleShape button_;

    // coordinates and dimensions of the button
    int width_;
    int height_;  
    int xPos_;
    int yPos_;
    
    Color color_; // button color
    Color colorOn_; // button color when the cursore is over it
    Text text_; // button text
    std::shared_ptr<Texture> texture_; // button texture
    std::shared_ptr<Texture> textureOn_;
};