#pragma once

#include "settings.h"

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

    Field(RenderWindow*, int, int);

    Field(const Field&);

    Field(Field&&);

    Field& operator=(const Field&);

    Field& operator=(Field&&);

    ~Field();

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

private:

    bool onField(int, int) const;

    void swap(const Field&);

private:

    RenderWindow* window_;
    RectangleShape field_;

    int x_;    // position and size of the fied
    int y_;
    int size_;

    unsigned int dataCounter_;
    unsigned short data_; // information about field status (free, taken or already used)

    Texture hitTexture_;
    Texture missTexture_;
};
