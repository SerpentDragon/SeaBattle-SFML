#pragma once

#include <memory>
#include "../Interface/settings.hpp"
#include "../Managers/TextureManager.hpp"

using namespace sf;

/*
data:
0 - the field is free
1 - the field is taken for the ship
2 - the field is free but there is a ship next to it
3 - hit ship
4 - hit field
*/
namespace field_data
{
    enum : unsigned short { free = 0, taken, ship_near, hit_ship, hit_field };
}

extern Texture playerHitTexture, playerMissedTexture, computerHitTexture, computerMissedTexture;

class Field
{
public:

    Field() = default;

    Field(std::shared_ptr<RenderWindow>, int, int);

    Field(const Field&) = default;

    Field(Field&&) noexcept = default;

    Field& operator=(const Field&) = default;

    Field& operator=(Field&&) noexcept = default;

    ~Field() = default;

public:

    void drawField() const;

    void resetData();

    const Vector2f getPosition() const;

    const int getData() const;

    void setData(int);

    void setCorrectColor();

    void setWrongColor();

    void setNeutralColor();

    void setCurrentColor(int);

    void displayHitTexture();

    void displayMissTexture();

    bool isChosen() const;

    void reinitField();

private:

    bool onField(int, int) const;

private:

    std::shared_ptr<RenderWindow> window_;
    RectangleShape field_;

    int x_;    // position and size of the fied
    int y_;
    int size_;

    unsigned int dataCounter_;
    unsigned short data_; // information about field status (free, taken or already used)

    std::shared_ptr<Texture> hitTexture_;
    std::shared_ptr<Texture> missTexture_;
};
