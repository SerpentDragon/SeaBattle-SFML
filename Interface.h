#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include "Mechanics.h"
#include "Button.h"
#include "Field.h"
#include "Ship.h"
#include "settings.h"

using namespace sf;

extern int screenWidth;

class Interface
{
    RenderWindow* window;

    mutable std::map<std::string, std::pair<Texture, RectangleShape>> img;

    inline void drawCoordinates(int, int, int) const;

public:
    Interface(const RenderWindow*);

    Interface(const Interface&) = delete;

    Interface(Interface&&) = delete;

    Interface& operator=(const Interface&) = delete;

    Interface& operator=(Interface&&) = delete;

    ~Interface();

    void mainMenu() const;

    void gameWindow() const;

    void showReference() const;

    void showMessage(const wchar_t*) const;
};
