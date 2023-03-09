#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
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

    Interface(const Interface&);

    Interface(Interface&&) noexcept;

    Interface& operator=(const Interface&);

    Interface& operator=(Interface&&) noexcept;

    ~Interface();

    void mainMenu() const;

    void gameWindow() const;

    void showReference() const;

    void showWarning() const;
};
