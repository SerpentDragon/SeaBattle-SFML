#pragma once

#include <SFML/Graphics.hpp>
#include "settings.h"

using namespace sf;

class Ship
{
    RenderWindow* window;

    int deckNumber;
    int x;
    int y;

    Texture shipTexture;
    RectangleShape shipRect;

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
};
