#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Button.h"
#include "Field.h"
#include "settings.h"

using namespace sf;

extern int screenWidth;

class Interface
{
    RenderWindow* window;

    mutable std::map<std::string, std::pair<Texture, RectangleShape>> img;

    inline void drawCoordinates(const int&, const int&, const int&) const;

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
};
