#pragma once

#include <SFML/Graphics.hpp>
#include "Managers/TextureManager.hpp"
#include "settings.h"
#include "Field.h"

using namespace sf;

class Ship
{
public:

    Ship(RenderWindow* window, int deck, int xPos, int yPos);

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

    const int getDeckNumber() const;

    const Vector2f getPosition() const;

    const bool isPlaced() const;

    void setIsPlaced(int);

    const int getWidth() const;

    const int getHeight() const;

    void setPosition(const Vector2f&);

    void setPosition(int, int);

private:

    void swap(const Ship&) noexcept;

    bool checkCells(const std::vector<Field>&, int) const;

    void locateShip(std::vector<Field>&, int);

    int calculatePosition() const;

private:

    RenderWindow* window_;

    // ship's parameters
    int deckNumber_;
    int x_;
    int y_;
    int width_;
    int height_;

    // is the ship is placed on the field
    int IsPlaced_;

    // number of rotations of the ship
    int rotateCounter_;

    Texture shipTexture_;
    Sprite shipRect_;

    // current coordinates
    int curr_x_;           
    int curr_y_;
};