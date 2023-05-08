#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <functional>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include <map>
#include "Mechanics.h"
#include "Button.h"
#include "Field.h"
#include "Ship.h"
#include "settings.h"

using namespace sf;

extern int screenWidth;

void timer();

bool comparator(const std::string&, const std::string);

class Interface
{
    RenderWindow* window;

    mutable std::map<std::string, std::pair<Texture, RectangleShape>> img;

    inline void drawCoordinates(int, int, int) const;

    void showMessage(const std::wstring&) const;

    bool showWarning(const std::wstring&) const;

    std::vector<std::string> readRecords() const;

    void writeRecords() const;

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

    void showRecords() const;
};
