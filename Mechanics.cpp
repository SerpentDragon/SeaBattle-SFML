#include "Mechanics.h"
#include <iostream>
#include <algorithm>

enum directions{up = 0, right, down, left};

void Mechanics::markTheDeck(int i, int j, std::vector<Field>* fieldArea) // should be optimized
{
    if (i - 1 >= 0)
    {
        if (j - 1 >= 0) 
        {
            (*fieldArea)[(i - 1) * 10 + j - 1].displayMissTexture();
            if (fieldArea == leftField) 
            {
                auto it = std::find(moves.begin(), moves.end(), (i - 1) * 10 + j - 1);
                if (it != moves.end()) moves.erase(it);
            }
        }
        if (j + 1 <= 9) 
        {
            (*fieldArea)[(i - 1) * 10 + j + 1].displayMissTexture();
            if (fieldArea == leftField) 
            {
                auto it = std::find(moves.begin(), moves.end(), (i - 1) * 10 + j + 1);
                if (it != moves.end()) moves.erase(it);
            }
        }
    }
    if (i + 1 <= 9)
    {
        if (j - 1 >= 0) 
        {
            (*fieldArea)[(i + 1) * 10 + j - 1].displayMissTexture();
            if (fieldArea == leftField) 
            {
                auto it = std::find(moves.begin(), moves.end(), (i + 1) * 10 + j - 1);
                if (it != moves.end()) moves.erase(it);
            }
        }
        if (j + 1 <= 9) 
        {
            (*fieldArea)[(i + 1) * 10 + j + 1].displayMissTexture();
            if (fieldArea == leftField) 
            {
                auto it = std::find(moves.begin(), moves.end(), (i + 1) * 10 + j + 1);
                if (it != moves.end()) moves.erase(it);
            }
        }
    }
}

void Mechanics::markKilledShip(int i, int j, std::vector<Field>* fieldArea, bool vertical, int deckNumber)
{
    int row = i;
    int column = j;
    int *coord = vertical ? &row : &column; // variable to change inside the loop
    int numberOfLoops = deckNumber == 1 ? 2 : 1; // check if killed ship had only 1 deck. In this case we should mark it up, left, right and down

    for(int num = 0; num < numberOfLoops; num++) // this loop allow go throught vertical and horizontal directions if the ship had 1 deck. Otherwise we will go just through one direction (vert or hor)
    {
        for(int direction = -1; direction < 2; direction += 2) // go through up/left and down/right directions
        {
            int limit = direction == -1 ? -1 : 10; // we cannot move away from area

            while((*coord) + direction != limit) // if we are still inside the area
            {
                *coord += direction;
                if ((*fieldArea)[row * 10 + column].getData() != 3) break; // if current field is not a hit deck of the ship
            }
            if ((*fieldArea)[row * 10 + column].getData() != 3) // check if this field is not a hit deck
            {
                (*fieldArea)[row * 10 + column].setData(4); // mark this field as hit
                (*fieldArea)[row * 10 + column].displayMissTexture(); // set appropriate texture

                auto it = std::find(moves.begin(), moves.end(), row * 10 + column);
                if (it != moves.end()) moves.erase(it);
            }

            row = i; column = j;
        }

        vertical = vertical ? false : true; // change direction (in case the ship had 1 deck)
        coord = vertical ? &row : &column;
    }
}

bool Mechanics::checkShipIsKilled(int i, int j, std::vector<Field>* fieldArea)
{
    int row = i;
    int column = j;
    int deckNumber = 1;

    bool vertical;

    for(int direction = up; direction <= left; direction++) // go through all directions
    {
        int limit = direction == up || direction == left ? -1 : 10; // we cannot move out the field (cells with numbers 0 and 9)
        int orientation = direction == up || direction == left ? -1 : 1; // we can move up or left (-1) and down or right (+1)
        int *coord = direction % 2 ? &row : &column; // choose variable to change depending on direction (vertical or horizontal)

        for(int k = 1; k < 4; k++)
        {
            if ((*coord) + orientation * k != limit) // if this position is inside the area
            {
                int tmp = *coord;
                *coord += orientation * k; 

                int fieldData = (*fieldArea)[row * 10 + column].getData();
                if (fieldData == 1) return false; // if the cell is still taken for a ship then this one wasn't killed but injured
                else if (fieldData == 2 || fieldData == 4) break; // if we meet 2 or 4 then we are out of the ship placement
                else if (fieldData == 3) 
                {
                    vertical = direction % 2 ? true : false; // if direction == up or direction == down
                    deckNumber++;
                }

                *coord = tmp;
            }
            else break;
        }

        row = i; column = j;
    }

    markKilledShip(i, j, fieldArea, vertical, deckNumber);

    if (fieldArea == leftField) playerShips--;
    else computerShips--;

    return true; // if all the cells of the ship were hit (and no one cell equals 1 what means this cell is taken for a ship) return true - the ship is killed
}

Mechanics::Mechanics(const RenderWindow *window, const std::vector<Field>* leftField, const std::vector<Field>* rightField)
{
    this->window = const_cast<RenderWindow*>(window);
    this->leftField = const_cast<std::vector<Field>*>(leftField);
    this->rightField = const_cast<std::vector<Field>*>(rightField);

    playerMove = true;
    playerShips = computerShips = 10;

    moves.resize(100);
    for(int i = 0; i < 100; i++) moves[i] = i;
}

Mechanics::~Mechanics()
{
    window = nullptr;
}

void Mechanics::placeComputerShips() const
{
    bool tmp = false;

    int i, j, k;

    int decks[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

    std::vector<int> direct;

    for(const int& deck : decks)
    {
        while(true)
        {
            i = rand() % 10; // choose the place for a ship
            j = rand() % 10;

            if ((*rightField)[i * 10 + j].getData()) continue; // if the place is taken, choose another

            if (deck == 1) // if the number of ship's deck is 1 we can place it
            {
                (*rightField)[i * 10 + j].setData(1);
                direct.emplace_back(0);
                break;
            }

            int tmpI = i, tmpJ = j;

            for(size_t direction = 0; direction < 4; direction++) // for every direction
            {
                int step; // we can move up or left (-1) and down or right (+1)
                int limit; // we cannot move out the field (cells with numbers 0 and 9)
                int *ptr = direction % 2 ? &tmpJ : &tmpI; // depending on the direction we must shange one of variables  

                if (direction == up || direction == left) step = limit = -1;
                else
                {
                    step = 1; // move in positive direction
                    limit = 10; // cannot step on the cell number 10 - it doesn't exist!
                }

                for(k = 0; k < deck && (*ptr) != limit; k++, (*ptr) += step)
                {
                    if ((*rightField)[tmpI * 10 + tmpJ].getData()) break; // if cell is taken - break 
                }
                if (k == deck) direct.emplace_back(direction); // if there number of free cells equals to deck we remember current direction

                tmpI = i; tmpJ = j;
            }

            if (direct.size()) break;
        }

        int num = rand() % direct.size(); // random direction   

        int step = direct[num] == up || direct[num] == left ? -1 : 1;
        int *ptr = direct[num] % 2 ? &j : &i;

        int leftXBorder = i - 1, leftYBorder = j - 1, rightXBorder = i + 1, rightYBorder = j + 1;

        if (direct[num] == up) leftXBorder = i - deck;
        else if (direct[num] == right) rightYBorder = j + deck;
        else if (direct[num] == down) rightXBorder = i + deck;
        else if (direct[num] == left) leftYBorder = j - deck;

        if (leftXBorder < 0) leftXBorder++;
        else if (rightXBorder > 9) rightXBorder--;
        if (leftYBorder < 0) leftYBorder++;
        else if (rightYBorder > 9) rightYBorder--;

        for(size_t m = leftXBorder; m <= rightXBorder; m++) // border the area where the ship will be placed
        {
            for(size_t n = leftYBorder; n <= rightYBorder; n++) (*rightField)[m * 10 + n].setData(2);
        }

        for(k = 0; k < deck; k++, (*ptr) += step) (*rightField)[i * 10 + j].setData(1); // place the ship

        direct.clear();
    }
}

bool Mechanics::startTheGame()
{
    bool flag = false;

    if (playerMove)
    {
        for(int i = 0; i < 100; i++)
        {
            if ((*rightField)[i].isChosen())
            {
                std::cout << "checkSize: " << possibleDirections.size() << std::endl;

                int fieldData = (*rightField)[i].getData();

                if (fieldData == 1) // if the ship is located here
                {
                    (*rightField)[i].setData(3);

                    (*rightField)[i].displayHitTexture(); 
                    markTheDeck(i / 10, i % 10, rightField);
                    
                    checkShipIsKilled(i / 10, i % 10, rightField);
                }
                else if (fieldData == 0 || fieldData == 2) // otherwise
                {
                    (*rightField)[i].displayMissTexture();
                    (*rightField)[i].setData(4);
                    playerMove = false;
                }
            }
        }
    }
    else
    {
        static int i, j;
        static int prevI, prevJ;

        if (possibleDirections.size())
        {
            std::cout << "pos: " << j << " " << i << std::endl;
            try
            {
                std::cout << "data: " << (*leftField)[i * 10 + j].getData() << std::endl;
            }
            catch(...)
            {

            }
            
            static int direct = -1;
            if (direct < 0) direct = possibleDirections[rand() % possibleDirections.size()];

            int step = direct == up || direct == left ? -1 : 1;
            int limit = direct == up || direct == left ? -1 : 10;
            int *coord = direct % 2 ? &j : &i;
            *coord += step;

            if (*coord == limit)
            {
                std::cout << "LIMIT!\n";
                possibleDirections.erase(find(possibleDirections.begin(), possibleDirections.end(), direct));
                direct = -1;
                i = prevI; j = prevJ;
            }
            else
            {
                std::cout << possibleDirections.size() << std::endl;

                for(const auto& el : possibleDirections)
                {
                    if (el == up) std::cout << "up";
                    else if(el == right) std::cout << "right";
                    else if (el == down) std::cout << "down";
                    else if (el == left) std::cout << "left";
                    std::cout << " ";
                }
                std::cout << std::endl;

                std::cout << "direction: " << direct << std::endl;


                int fieldData = (*leftField)[i * 10 + j].getData();

                auto it = std::find(moves.begin(), moves.end(), i * 10 + j);
                if (it != moves.end()) moves.erase(it);

                if (fieldData == 1)
                {
                    std::cout << "GOAL\n";
                    for(int i = 0; i < possibleDirections.size(); i++)
                    {
                        if (possibleDirections[i] % 2 != direct % 2)
                        {
                            possibleDirections.erase(possibleDirections.begin() + i);
                            i--;
                        }
                    }

                    (*leftField)[i * 10 + j].setData(3);

                    RectangleShape shipsDeck(Vector2f(fieldSize, fieldSize));
                    shipsDeck.setTexture(&computerHitTexture);
                    shipsDeck.setPosition((*leftField)[i * 10 + j].getPosition());
                    hitPositions.emplace_back(shipsDeck);

                    markTheDeck(i, j, leftField);

                    if (checkShipIsKilled(i, j, leftField)) possibleDirections.clear();
                }
                else
                {
                    std::cout << "MISSED\n";
                    (*leftField)[i * 10 + j].displayMissTexture();
                    moves.erase(std::find(moves.begin(), moves.end(), i * 10 + j));

                    possibleDirections.erase(find(possibleDirections.begin(), possibleDirections.end(), direct));
                    direct = -1;
                    i = prevI; j = prevJ;
                    playerMove = true;

                    std::cout << "prevPos: " << j << " " << i << std::endl;
                    for(const auto& el : possibleDirections)
                    {
                        if (el == up) std::cout << "up";
                        else if(el == right) std::cout << "right";
                        else if (el == down) std::cout << "down";
                        else if (el == left) std::cout << "left";
                        std::cout << " ";
                    }
                    std::cout << std::endl;

                }

                flag = true;
            }           
        }
        else
        {
            std::cout << "NEW MOVE!\n";
            int fieldNum = moves[rand() % moves.size()];
            moves.erase(std::find(moves.begin(), moves.end(), fieldNum));

            i = fieldNum / 10;
            j = fieldNum % 10;

            std::cout << j << " " << i << std::endl;

            int fieldData = (*leftField)[i * 10 + j].getData();
            if (fieldData == 1)
            {
                prevI = i; prevJ = j;

                std::cout << "rememberPOS: " << prevJ << " " << prevI << std::endl;

                (*leftField)[i * 10 + j].setData(3);

                RectangleShape shipsDeck(Vector2f(fieldSize, fieldSize));
                shipsDeck.setTexture(&computerHitTexture);
                shipsDeck.setPosition((*leftField)[i * 10 + j].getPosition());
                hitPositions.emplace_back(shipsDeck);

                markTheDeck(i, j, leftField);

                if (!checkShipIsKilled(i, j, leftField))
                {
                    if (i - 1 >= 0) possibleDirections.emplace_back(up);
                    if (j + 1 <= 9) possibleDirections.emplace_back(right);
                    if (i + 1 <= 9) possibleDirections.emplace_back(down);
                    if (j - 1 >= 0) possibleDirections.emplace_back(left);
                }
            }
            else if (fieldData == 0 || fieldData == 2)
            {
                (*leftField)[i * 10 + j].displayMissTexture();
                playerMove = true;
            }  

            flag = true;  
        }
    }

    return flag;
}

void Mechanics::drawPositions() const
{
    for(const auto& position : hitPositions) window->draw(position);
}

bool Mechanics::checkEndGame() const
{
    return playerShips == 0 || computerShips == 0;
}

const wchar_t* Mechanics::getResult() const
{
    if (!playerShips) return L"Поражение!";
    return L"Победа!";
}
