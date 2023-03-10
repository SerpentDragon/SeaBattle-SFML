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

public:

    Mechanics(const RenderWindow *window, const std::vector<Field>* leftField, const std::vector<Field>* rightField);

    Mechanics(const Mechanics& obj) = delete;
    
    Mechanics(Mechanics&& obj) = delete;

    Mechanics& operator=(const Mechanics& obj) = delete;

    Mechanics& operator=(Mechanics&& obj) = delete;

    ~Mechanics();

    void placeComputerShips() const;

    void startTheGame();
};