#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <filesystem>
#include <functional>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex>
#include "Managers/TextureManager.hpp"
#include "Mechanics.h"
#include "Button.h"
#include "Field.h"
#include "Ship.h"
#include "settings.h"

using namespace sf;

extern int screenWidth;

class Interface
{
public:

    Interface(RenderWindow*);

    Interface(const Interface&) = delete;

    Interface(Interface&&) = delete;

    Interface& operator=(const Interface&) = delete;

    Interface& operator=(Interface&&) = delete;

    ~Interface();

    void mainMenu() const;

    void gameWindow() const;

    void showReference() const;

    void showRecords() const;

private:

    void drawCoordinates(int, int, int) const;

    void showMessage(const std::wstring&) const;

    bool showWarning(const std::wstring&) const;

    std::vector<std::string> readRecords() const;

    void writeRecords() const;

private:

    RenderWindow* window_;

    mutable std::unordered_map<std::string, RectangleShape> img_;

};
