#pragma once

#include <array>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <fstream>
#include <fstream>
#include <filesystem>
#include "Gameplay.hpp"
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../Elements/Ship.hpp"
#include "../Managers/Timer.hpp"
#include "../Elements/Field.hpp"
#include "../Widgets/Button.hpp"
#include "../Interface/settings.hpp"
#include "../Managers/FileIOManager.hpp"
#include "../Managers/TextureManager.hpp"

using namespace sf;

class Interface
{
public:

    Interface(RenderWindow*);

    Interface(const Interface&) = delete;

    Interface(Interface&&) = delete;

    Interface& operator=(const Interface&) = delete;

    Interface& operator=(Interface&&) = delete;

    ~Interface();

    void mainMenu();

    void gameWindow();

    void showReference() const;

    void showRecords() const;

private:

    void initCoordinatesText();

    void initTextures();

    void createMainWindowButtons();

    void createGameWindowButtons();

    void createGameFields();

    void createShips();

    void initGlobalTimeText();

private:

    void reinitGameParams();

    void drawCoordinates(int, int, int) const;

    void showMessage(const std::wstring&) const;

    bool showWarning(const std::wstring&) const;

private:

    RenderWindow* window_;

    // textures
    mutable std::unordered_map<std::string, RectangleShape> img_;

    // coordinates of the game fields
    mutable std::array<Text, 20> coordinates_;

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
