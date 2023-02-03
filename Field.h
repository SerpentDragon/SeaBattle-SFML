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

    template<typename T>
    void Swap(T&&);

public:
    Field(const RenderWindow*, const int&, const int&, const int&);

    Field(const Field&);

    Field(Field&&);

    Field& operator=(const Field&);

    Field& operator=(Field&&);

    ~Field();

    void drawField() const;
};