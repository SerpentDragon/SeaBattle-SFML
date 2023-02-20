#pragma once

#include "settings.h"

using namespace sf;

extern int screenWidth;

class Field
{
    RenderWindow* window;
    RectangleShape field;

    int x;
    int y;
    int size;

    int data;

    template<typename T>
    void Swap(T&&);

public:
    Field(const RenderWindow*, const int&, const int&);

    Field(const Field&);

    Field(Field&&);

    Field& operator=(const Field&);

    Field& operator=(Field&&);

    ~Field();

    void drawField() const;

    const Vector2f getPosition() const;

    const int getData() const;

    void setData(const int&);

    void setCorrectColor();

    void setWrongColor();

    void setNeutralColor();
};