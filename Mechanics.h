#pragma once

#include <vector>
#include "Field.h"
#include "settings.h"

class Mechanics
{
    RenderWindow *window;

    bool playerMove;

    int playerShips;
    int computerShips;

    std::vector<int> moves;

    std::vector<RectangleShape> hitPositions;

    std::vector<Field>* leftField;
    std::vector<Field>* rightField;

    void markTheDeck(int, int, std::vector<Field>*);

    bool checkShipIsKilled(int, int, std::vector<Field>*);

    void markKilledShip(int, int, std::vector<Field>*, bool, int);

public:

    Mechanics(const RenderWindow*, const std::vector<Field>*, const std::vector<Field>*);

    Mechanics(const Mechanics&) = delete;
    
    Mechanics(Mechanics&&) = delete;

    Mechanics& operator=(const Mechanics&) = delete;

    Mechanics& operator=(Mechanics&&) = delete;

    ~Mechanics();

    void placeComputerShips() const;

    void startTheGame();
};