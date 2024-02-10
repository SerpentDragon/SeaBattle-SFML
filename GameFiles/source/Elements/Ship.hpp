#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "../Elements/Field.hpp"
#include "../Interface/settings.hpp"
#include "../Managers/TextureManager.hpp"

using namespace sf;

class Ship
{
public:

    Ship(std::shared_ptr<RenderWindow>, int, int, int);

    Ship(const Ship&) = default;

    Ship(Ship&&) noexcept = default;

    Ship& operator=(const Ship&) = default;

    Ship& operator=(Ship&&) noexcept = default;

    ~Ship() = default;

public:

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

    bool checkCells(const std::vector<Field>&, int) const;

    void locateShip(std::vector<Field>&, int);

    int calculatePosition() const;

private:

    std::shared_ptr<RenderWindow> window_;

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

    std::shared_ptr<Texture> shipTexture_;
    Sprite shipRect_;

    // current coordinates
    int curr_x_;           
    int curr_y_;
};