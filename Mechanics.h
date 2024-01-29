#pragma once

#include <algorithm>
#include <vector>
#include <array>
#include "Ship.h"
#include "Field.h"
#include "settings.h"

class Mechanics
{
public:

    Mechanics(RenderWindow*, std::vector<Field>*, std::vector<Field>*);

    Mechanics(const Mechanics&) = delete;
    
    Mechanics(Mechanics&&) = delete;

    Mechanics& operator=(const Mechanics&) = delete;

    Mechanics& operator=(Mechanics&&) = delete;

    ~Mechanics();

    void placeShips(std::vector<Field>*, std::vector<Ship>*) const;

    bool startTheGame();

    void drawPositions() const;

    bool checkEndGame() const;

    std::wstring getResult() const;

private:

    void markTheDeck(int, int, std::vector<Field>*);

    bool checkShipIsKilled(int, int, std::vector<Field>*);

    void markKilledShip(const std::vector<int>&, std::vector<Field>*);

    void drawSurvivedShips() const;

private:

    RenderWindow* window_;

    bool playerMove_; // flag to check whether a user or a computer is making a move now

    int playerShips_;   // counter of player's ships
    int computerShips_; // counter of computer's ships

    std::vector<int> possibleDirections_; // possible directions to check ariund the ship to destroy it

    std::vector<int> moves_; // list of comupter's move. Keep it to not hit a field which already hit

    std::vector<RectangleShape> hitPositions_; // textures to mark player's hit ships

    std::vector<Field>* leftField_;  // player's field
    std::vector<Field>* rightField_; // computer's field

    mutable std::vector<Ship> shipList_;
};