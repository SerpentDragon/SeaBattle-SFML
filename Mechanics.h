#pragma once

#include <vector>
#include "Field.h"
#include "settings.h"

class Mechanics
{
    bool playerMove;

    int playerShips;
    int computerShips;

    std::vector<std::pair<int, int>> moves;

public:
    Mechanics()
    {
        playerMove = true;
        playerShips = computerShips = 10;
    }

    ~Mechanics()
    {
        moves.clear();
    }

    void placeComputerShips(std::vector<Field>&) const;

    void startTheGame(std::vector<Field>&, std::vector<Field>&);
};