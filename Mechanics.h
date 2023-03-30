#pragma once

#include <algorithm>
#include <vector>
#include <array>
#include "Field.h"
#include "settings.h"

class Mechanics
{
    RenderWindow *window;

    bool playerMove; // flag to check whether a user or a computer is making a move now

    int playerShips;   // counter of player's ships
    int computerShips; // counter of computer's ships

    std::vector<int> possibleDirections;

    std::vector<int> moves; // list of comupter's move. Keep it to not hit a field which already hit

    std::vector<RectangleShape> hitPositions; // textures to mark player's hit ships

    std::vector<Field>* leftField;  // player's field
    std::vector<Field>* rightField; // computer's field

    void markTheDeck(int, int, std::vector<Field>*);

    bool checkShipIsKilled(int, int, std::vector<Field>*);

    void markKilledShip(const std::vector<int>&, std::vector<Field>*);

public:

    Mechanics(const RenderWindow*, const std::vector<Field>*, const std::vector<Field>*);

    Mechanics(const Mechanics&) = delete;
    
    Mechanics(Mechanics&&) = delete;

    Mechanics& operator=(const Mechanics&) = delete;

    Mechanics& operator=(Mechanics&&) = delete;

    ~Mechanics();

    void placeComputerShips() const;

    bool startTheGame();

    void drawPositions() const;

    bool checkEndGame() const;

    std::wstring getResult() const;
};