#pragma once

#include <SFML/Graphics.hpp>
#include "settings.h"
#include "Field.h"

using namespace sf;

class Ship
{
    RenderWindow* window;

    int deckNumber;
    int x;
    int y;
    int width;
    int height;

    Texture shipTexture;
    Sprite shipRect;
    int XPos;
    int YPos;

    template <typename T>
    void Swap(T&&) noexcept;

public:
    Ship(const RenderWindow* window, const int& deck, const int& xPos, const int& yPos);

    Ship(const Ship& obj);

    Ship(Ship&& obj) noexcept;

    Ship& operator=(const Ship& obj);

    Ship& operator=(Ship&& obj) noexcept;

    ~Ship();

    void drawShip() const;

    bool onShip(const int& x, const int& y) const;  

    void rotateShip(const int&, const int&);

    void setFieldPosition(std::vector<Field>&, const int& = 0);

    const Vector2f getPos() const;

    const int getWidth() const;

    const int getHeight() const;

    void setPos(const int&, const int&);
};