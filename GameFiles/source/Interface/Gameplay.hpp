#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include "settings.hpp"
#include "../Elements/Ship.hpp"
#include "../Elements/Field.hpp"
#include "../Managers/Localizer.hpp"

class Gameplay
{
public:

    Gameplay(std::shared_ptr<RenderWindow>, std::vector<Field>*, std::vector<Field>*);

    Gameplay(const Gameplay&) = delete;
    
    Gameplay(Gameplay&&) = default;

    Gameplay& operator=(const Gameplay&) = delete;

    Gameplay& operator=(Gameplay&&) = default;

    ~Gameplay();

public:

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

    std::shared_ptr<RenderWindow> window_;

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