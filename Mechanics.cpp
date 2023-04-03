#include "Mechanics.h"

#include <iostream>

enum directions{up = 0, right, down, left};

void Mechanics::markTheDeck(int i, int j, std::vector<Field>* fieldArea)
{
    for(int row = -1; row <= 1; row += 2)
    {
        int rowLimit = row == -1 ? -1 : 10; // limit not to go beyond field area (row must not be equal to -1 or 10)

        for(int column = -1; column <= 1; column += 2)
        {
            int columnLimit = column == -1 ? -1 : 10; // limit not to go beyond field area (column must not be equal to -1 or 10)

            if (i + row != rowLimit && j + column != columnLimit) // check corners of the deck
            {
                (*fieldArea)[(i + row) * 10 + j + column].displayMissTexture();
                if (fieldArea == leftField)
                {
                    auto it = std::find(moves.begin(), moves.end(), (i + row) * 10 + j + column);
                    if (it != moves.end()) moves.erase(it);
                }
            }
        }
    }
}

void Mechanics::markKilledShip(const std::vector<int>& positions, std::vector<Field>* fieldArea)
{
    for(const auto& position : positions) // check all chosen positions around the ship
    {
        (*fieldArea)[position].setData(4); // mark this field as used
        (*fieldArea)[position].displayMissTexture(); // display appropriate texture
        
        if (fieldArea == leftField) // if the field is located on player's field
        {
            auto it = std::find(moves.begin(), moves.end(), position); // erase this field from the list of afordable moves
            if (it != moves.end()) moves.erase(it);
        }
        
    }
}

bool Mechanics::checkShipIsKilled(int i, int j, std::vector<Field>* fieldArea)
{
    bool vertical = false, horizontal = false; // variables to check ship's orientation 
    int row, column, *coord;

    std::vector<int> positions; // positions to mark as used

    for(int direct = up; direct <= left; direct++) // check all directions
    {
        row = i; column = j;

        int step = direct == up || direct == left ? -1 : 1; // move up or left: -1; down or right: +1
        int limit = direct == up || direct == left ? -1 : 10; // can not move away the area
        coord = direct % 2 ? &column : &row; // coordinate to change
        *coord += step;

        if (*coord == limit) continue; // if moved away the area

        int fieldData = (*fieldArea)[row * 10 + column].getData(); // get data of the field
        if (fieldData == 1 || fieldData == 3) // if ship or wrecked deck is located here
        {
            direct % 2 ? horizontal = true : vertical = true; // remember orientation of the ship
            break;
        }
    }

    row = i; column = j; // restore current position after changing using the pointer

    if (!vertical && !horizontal) // if vertical == false and horizontal == false => this ship has the only deck
    {
        if (i - 1 >= 0) positions.emplace_back((i - 1) * 10 + j); // positions around this ship must be marked as used because no one ship can be here
        if (i + 1 <= 9) positions.emplace_back((i + 1) * 10 + j);
        if (j - 1 >= 0) positions.emplace_back(i * 10 + j - 1);
        if (j + 1 <= 9) positions.emplace_back(i * 10 + j + 1);
    }
    else
    {
        coord = vertical ? &row : &column; // choose an orientation
        for(int step = -1; step <= 1; step += 2) // check right and left or up and down directions
        {
            row = i; column = j;

            int limit = step == -1 ? -1 : 10; // can not move away the area
           
            while(true)
            {
                *coord += step;
                if (*coord == limit) break; // if moved away the area

                int fieldData = (*fieldArea)[row * 10 + column].getData(); // get data of the field
                if (fieldData == 1) return false; // if data is 1 - this ship is not destroued yed
                else if (fieldData != 3) // if data is not 1 or 3 - this positions must be marked as used
                {
                    positions.emplace_back(row * 10 + column);
                    break;
                }
            }
        }
    }

    markKilledShip(positions, fieldArea); // mark fields around this ship as used because no ship can be here

    if (fieldArea == leftField) playerShips--; // if this ship was destroyed on the player's field - decrease number of player's ships
    else computerShips--; // otherwise decrease number of computer's ships

    return true;
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

void Mechanics::placeShips(std::vector<Field>* fieldArea, std::vector<Ship>* ships) const
{
    bool tmp = false;

    int i, j, k;

    int decks[] = {1, 1, 1, 1, 2, 2, 2, 3, 3, 4};

    std::vector<int> direct;

    for(int count = 0; count < 10; count++)
    {
        while(true)
        {
            i = rand() % 10; // choose the place for a ship
            j = rand() % 10;

            if ((*fieldArea)[i * 10 + j].getData()) continue; // if the place is taken, choose another

            if (decks[count] == 1) // if the number of ship's deck is 1 we can place it
            {
                (*fieldArea)[i * 10 + j].setData(1);
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

                for(k = 0; k < decks[count] && (*ptr) != limit; k++, (*ptr) += step)
                {
                    if ((*fieldArea)[tmpI * 10 + tmpJ].getData()) break; // if cell is taken - break 
                }
                if (k == decks[count]) direct.emplace_back(direction); // if there number of free cells equals to deck we remember current direction

                tmpI = i; tmpJ = j;
            }

            if (direct.size()) break;
        }

        int num = rand() % direct.size(); // random direction   

        int step = direct[num] == up || direct[num] == left ? -1 : 1;
        int *ptr = direct[num] % 2 ? &j : &i;

        int leftXBorder = i - 1, leftYBorder = j - 1, rightXBorder = i + 1, rightYBorder = j + 1;

        if (direct[num] == up) leftXBorder = i - decks[count];
        else if (direct[num] == right) rightYBorder = j + decks[count];
        else if (direct[num] == down) rightXBorder = i + decks[count];
        else if (direct[num] == left) leftYBorder = j - decks[count];

        if (leftXBorder < 0) leftXBorder++;
        else if (rightXBorder > 9) rightXBorder--;
        if (leftYBorder < 0) leftYBorder++;
        else if (rightYBorder > 9) rightYBorder--;

        for(size_t m = leftXBorder; m <= rightXBorder; m++) // border the area where the ship will be placed
        {
            for(size_t n = leftYBorder; n <= rightYBorder; n++) (*fieldArea)[m * 10 + n].setData(2);
        }

        if (fieldArea == leftField)
        {
            if (direct[num] % 2) (*ships)[count].rotateShip((*ships)[count].getPosition().x, (*ships)[count].getPosition().y);

            if (direct[num] == up)
            {
                (*ships)[count].setPosition((*fieldArea)[(i - decks[count] + 1) * 10 + j].getPosition());
            }
            else if (direct[num] == right || direct[num] == down)
            {
                (*ships)[count].setPosition((*fieldArea)[i * 10 + j].getPosition());
            }
            else if (direct[num] == left)
            {
                (*ships)[count].setPosition((*fieldArea)[i * 10 + j - decks[count] + 1].getPosition());
            }

            (*ships)[count].setIsPlaced(i * 10 + j);
        }

        for(k = 0; k < decks[count]; k++, (*ptr) += step) (*fieldArea)[i * 10 + j].setData(1); // place the ship

        direct.clear();
    }
}

bool Mechanics::startTheGame()
{
    bool flag = false;

    if (playerMove)
    {
        for(int k = 0; k < 100; k++)
        {
            if ((*rightField)[k].isChosen())
            {
                int fieldData = (*rightField)[k].getData();
                if (fieldData == 1)
                {
                    (*rightField)[k].setData(3);
                    (*rightField)[k].displayHitTexture();

                    markTheDeck(k / 10, k % 10, rightField);
                    checkShipIsKilled(k / 10, k % 10, rightField);
                }
                else if (fieldData == 0 || fieldData == 2)
                {
                    (*rightField)[k].setData(4);
                    (*rightField)[k].displayMissTexture();
                    playerMove = false;
                }
                break;
            }
        }
    }
    else
    {
        static int i, j;
        static int prevI, prevJ;

        if (possibleDirections.size())
        {
            static int currentDirection = -1;
            if (currentDirection == -1) currentDirection = possibleDirections[rand() % possibleDirections.size()];

            int *coord = currentDirection % 2 ? &j : &i;
            int step = currentDirection == up || currentDirection == left ? -1 : 1;
            int limit = currentDirection == up || currentDirection == left ? -1 : 10;

            *coord += step;
            if (*coord == limit)
            {
                possibleDirections.erase(std::find(possibleDirections.begin(), possibleDirections.end(), currentDirection));
                currentDirection = -1;
                i = prevI; j = prevJ;
                return false;
            }

            auto check = std::find(moves.begin(), moves.end(), i * 10 + j);

            int fieldData = (*leftField)[i * 10 + j].getData();
            if (fieldData == 1 || fieldData == 3)
            {
                for(auto it = possibleDirections.begin(); it != possibleDirections.end(); it++)
                {
                    if (*it % 2 != currentDirection % 2)
                    {
                        possibleDirections.erase(it);
                        it--;
                    }
                }

                if (fieldData == 1)
                {
                    (*leftField)[i * 10 + j].setData(3);
                    moves.erase(check);

                    markTheDeck(i, j, leftField);
                    if (checkShipIsKilled(i, j, leftField))
                    {
                        possibleDirections.clear();
                        currentDirection = -1;
                    }

                    RectangleShape hitPos(Vector2f(fieldSize, fieldSize));
                    hitPos.setPosition((*leftField)[i * 10 + j].getPosition());
                    hitPos.setTexture(&computerHitTexture);

                    hitPositions.emplace_back(hitPos);
                }
            }
            else 
            {
                if (check != moves.end())
                {
                    moves.erase(check);

                    (*leftField)[i * 10 + j].setData(4);
                    (*leftField)[i * 10 + j].displayMissTexture();
                    playerMove = true;
                }

                possibleDirections.erase(std::find(possibleDirections.begin(), possibleDirections.end(), currentDirection));
                currentDirection = -1;
                i = prevI; j = prevJ;
            }
        }
        else
        {
            int k = moves[rand() % moves.size()];
            int fieldData = (*leftField)[k].getData();

            i = k / 10; prevI = i;
            j = k % 10; prevJ = j;

            auto it = std::find(moves.begin(), moves.end(), k);
            if (it != moves.end()) moves.erase(it);

            if (fieldData == 1)
            {
                (*leftField)[k].setData(3);

                markTheDeck(i, j, leftField);

                RectangleShape hitPos(Vector2f(fieldSize, fieldSize));
                hitPos.setPosition((*leftField)[k].getPosition());
                hitPos.setTexture(&computerHitTexture);

                hitPositions.emplace_back(hitPos);

                if (!checkShipIsKilled(i, j, leftField))
                {
                    std::array<int, 2> taken{3, 4};

                    if (i - 1 >= 0) 
                    {
                        if (std::find(taken.begin(), taken.end(), (*leftField)[(i - 1) * 10 + j].getData()) == taken.end())
                            possibleDirections.emplace_back(up);
                    }
                    if (i + 1 <= 9) 
                    {
                        if (std::find(taken.begin(), taken.end(), (*leftField)[(i + 1) * 10 + j].getData()) == taken.end())
                            possibleDirections.emplace_back(down);
                    }
                    if (j - 1 >= 0) 
                    {
                        if (std::find(taken.begin(), taken.end(), (*leftField)[i * 10 + j - 1].getData()) == taken.end())
                            possibleDirections.emplace_back(left);
                    }
                    if (j + 1 <= 9) 
                    {
                        if (std::find(taken.begin(), taken.end(), (*leftField)[i * 10 + j + 1].getData()) == taken.end())
                            possibleDirections.emplace_back(right);
                    }
                }
            }
            else if (fieldData == 0 || fieldData == 2)
            {
                (*leftField)[k].setData(4);
                (*leftField)[k].displayMissTexture();
                playerMove = true;
            }
        }

        flag = true;
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

std::wstring Mechanics::getResult() const
{
    if (!playerShips) return L"Поражение!";
    return L"Победа!";
}
