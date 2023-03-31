#pragma once

#include <SFML/Graphics.hpp>
#include "settings.h"
#include "Field.h"

using namespace sf;

class Ship
{
    RenderWindow* window;

    int deckNumber;
    int x;              // starting coordinates
    int y;
    int width;
    int height;

    int IsPlaced;       // is the ship is placed on the field

    int rotateCounter;  // number of rotations of the ship

    Texture shipTexture;
    Sprite shipRect;
    int XPos;           // current coordinates
    int YPos;

    template <typename T>
    void Swap(T&&) noexcept;

    bool checkCells(const std::vector<Field>&, int) const;

    void locateShip(std::vector<Field>&, int);

    int calculatePosition() const;

public:
    Ship(const RenderWindow* window, int deck, int xPos, int yPos);

    Ship(const Ship& obj);

    Ship(Ship&& obj) noexcept;

    Ship& operator=(const Ship& obj);

    Ship& operator=(Ship&& obj) noexcept;

    ~Ship();

    void drawShip() const;

    bool onShip(int, int) const;  

    void rotateShip(int, int);

    void setFieldColor(std::vector<Field>&);

    void placeShip(std::vector<Field>&);

    void resetPosition(std::vector<Field>&);

    const Vector2f getPosition() const;

    const bool isPlaced() const;

    void setIsPlaced(int);

    const int getWidth() const;

    const int getHeight() const;

    void setPosition(const Vector2f&);

    void setPosition(int, int);
};