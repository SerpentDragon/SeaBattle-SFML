#pragma once

#include "settings.h"

using namespace sf;

extern Texture playerHitTexture, playerMissedTexture, computerHitTexture, computerMissedTexture;

class Field
{
    RenderWindow* window;
    RectangleShape field;

    int x;    // position and size of the fied
    int y;
    int size;

    int data; // information about field status (free, taken or already used)

    Texture hit;
    Texture miss;

    bool onField(int, int) const;

    template<typename T>
    void Swap(T&&);

public:
    Field(const RenderWindow*, int, int);

    Field(const Field&);

    Field(Field&&);

    Field& operator=(const Field&);

    Field& operator=(Field&&);

    ~Field();

    void drawField() const;

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
};
