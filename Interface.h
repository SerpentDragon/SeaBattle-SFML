#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.h"
#include "Field.h"
#include "settings.h"

using namespace sf;

class Interface
{
    RenderWindow* window;

public:
    Interface(const RenderWindow*);

    Interface(const Interface&);

    Interface(Interface&&);

    Interface& operator=(const Interface&);

    Interface& operator=(Interface&&);

    ~Interface();

    void mainMenu() const;

    void gameWindow() const;

    void showReference() const;
};
