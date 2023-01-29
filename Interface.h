#pragma once

#include <SFML/Graphics.hpp>
#include "Button.h"
#include "settings.h"

using namespace sf;

class Interface
{
    RenderWindow* window;

public:
    Interface(RenderWindow*);

    Interface(const Interface&);

    Interface(Interface&&);

    Interface& operator=(const Interface&);

    Interface& operator=(Interface&&);

    ~Interface();

    void mainMenu();

    void showReference();
};
