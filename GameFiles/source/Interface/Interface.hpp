#pragma once

#include <regex>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <functional>
#include <filesystem>
#include "Gameplay.hpp"
#include "settings.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Elements/Ship.hpp"
#include "../Managers/Timer.hpp"
#include "../Elements/Field.hpp"
#include "../Widgets/Button.hpp"
#include "../Managers/FileIOManager.hpp"
#include "../Managers/TextureManager.hpp"

using namespace sf;

extern int screenWidth;

class Interface
{
public:

    Interface(std::shared_ptr<RenderWindow>);

    Interface(const Interface&) = delete;

    Interface(Interface&&) = delete;

    Interface& operator=(const Interface&) = delete;

    Interface& operator=(Interface&&) = delete;

    ~Interface() = default;

public:

    void mainMenu();

    void gameWindow();

    void showReference() const;

    void showRecords() const;

private:

    void initTextures();

    void createMainWindowButtons();

    void createGameWindowButtons();

    void initCoordinatesText();

    void createShips();

    void initGlobalTimeText();

    void createFields();

private: 

    void drawCoordinates(int, int, int) const;

    void reinitGameParams();

    void showMessage(const std::wstring&) const;

    bool showWarning(const std::wstring&) const;

private:

    std::shared_ptr<RenderWindow> window_;

    // textures
    mutable std::unordered_map<std::string, RectangleShape> img_;

    // coordinates of the game fields
    mutable std::array<Text, 20> coordinates_;

    // timer params
    Timer timer_;
    Text timeText_;

    // main window buttons
    Button playButton_;
    Button referenceButton_;
    Button recordsButton_;
    Button exitButton_;

    // game window buttons
    Button startButton_;
    Button autoButton_;
    Button quitButton_;

    // game fields
    std::vector<Field> leftField_;
    std::vector<Field> rightField_;

    // ships
    std::vector<Ship> ships_;

    Gameplay gameplay_;
    FileIOManager file_;
};
