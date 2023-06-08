#include "Mechanics.h"

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
    int deckCounter = 0;

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
                if (fieldData == 1) return false; // if data is 1 - this ship is not destroued yet
                else if (fieldData == 3) deckCounter++;
                else // if data is not 1 or 3 - this positions must be marked as used
                {
                    positions.emplace_back(row * 10 + column);
                    break;
                }
            }
        }
    }

    markKilledShip(positions, fieldArea); // mark fields around this ship as used because no ship can be here

    if (fieldArea == leftField) playerShips--; // if this ship was destroyed on the player's field - decrease number of player's ships
    else 
    {
        computerShips--; // otherwise decrease number of computer's ships

        for(auto it = shipList.begin(); it != shipList.end(); it++)
        {
            if (it->getDeckNumber() == deckCounter)
            {
                shipList.erase(it);
                break;
            }
        }
    }

    return true;
}

void Mechanics::drawSurvivedShips() const
{
    for(const auto& ship : shipList) ship.drawShip();
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

    int decks[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};

    std::vector<int> direct;

    for(int count = 0; count < 10; count++)
    {
        while(true)
        {           
            i = rand() % 10; // choose the place for a ship
            j = rand() % 10;

            if (decks[count] == 1) // if the number of ship's deck is 1 we can place it
            {
                if ((*fieldArea)[i * 10 + j].getData()) continue;

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

        Ship *shipPtr;

        if (fieldArea == leftField) 
        {
            shipPtr = &(*ships)[9 - count];
            shipPtr->setIsPlaced(i * 10 + j);
        }
        else
        {
            shipList.emplace_back((*ships)[9 - count]);
            shipList[count].setIsPlaced(-1);
            shipList[count].resetPosition(*leftField);

            shipPtr = &shipList[count];
        }

        if (direct[num] % 2) shipPtr->rotateShip(shipPtr->getPosition().x, shipPtr->getPosition().y);
        if (direct[num] == up)
        {
            shipPtr->setPosition((*fieldArea)[(i - decks[count] + 1) * 10 + j].getPosition());
        }
        else if (direct[num] == right || direct[num] == down)
        {
            shipPtr->setPosition((*fieldArea)[i * 10 + j].getPosition());
        }
        else if (direct[num] == left)
        {
            shipPtr->setPosition((*fieldArea)[i * 10 + j - decks[count] + 1].getPosition());
        }

        for(k = 0; k < decks[count]; k++, (*ptr) += step) (*fieldArea)[i * 10 + j].setData(1); // place the ship

        direct.clear();
    }
}

bool Mechanics::startTheGame()
{
    bool flag = false; // check if the current move is for player or computer

    if (playerMove) // if player's turn
    {
        for(int k = 0; k < 100; k++)
        {
            if ((*rightField)[k].isChosen()) // choose field
            {
                int fieldData = (*rightField)[k].getData();
                if (fieldData == 1) // if the ship is located here
                {
                    (*rightField)[k].setData(3); // mark deck as destroyed
                    (*rightField)[k].displayHitTexture(); // set appropriate texture

                    markTheDeck(k / 10, k % 10, rightField); // mark corners of the deck
                    checkShipIsKilled(k / 10, k % 10, rightField); // check whether the ships is destroyed or not
                }
                else if (fieldData == 0 || fieldData == 2) // otherwise
                {
                    (*rightField)[k].setData(4); // mark deck as used
                    (*rightField)[k].displayMissTexture(); // set appropriate texture
                    playerMove = false; // the next move is for computer
                }
                break;
            }
        }
    }
    else
    {
        static int i, j;
        static int prevI, prevJ;

        flag = true;

        if (possibleDirections.size()) // if there are directions around the ship to check
        {
            static int currentDirection = -1;
            if (currentDirection == -1) currentDirection = possibleDirections[rand() % possibleDirections.size()]; // choose random direction

            int *coord = currentDirection % 2 ? &j : &i; // veriable to change 
            int step = currentDirection == up || currentDirection == left ? -1 : 1; // can move left/up: -1; right/down: +1
            int limit = currentDirection == up || currentDirection == left ? -1 : 10; // can't move away the area

            *coord += step;
            if (*coord == limit) // if moved away the field area
            {
                possibleDirections.erase(std::find(possibleDirections.begin(), possibleDirections.end(), currentDirection)); // erase current direction
                currentDirection = -1;
                i = prevI; j = prevJ; // restore start position
                return false;
            }

            auto check = std::find(moves.begin(), moves.end(), i * 10 + j);

            int fieldData = (*leftField)[i * 10 + j].getData(); // check filed's data
            if (fieldData == 1 || fieldData == 3) // if there is a ship or destroyed deck
            {
                for(auto it = possibleDirections.begin(); it != possibleDirections.end(); it++) // erase opposite directions
                {
                    if (*it % 2 != currentDirection % 2)
                    {
                        possibleDirections.erase(it);
                        it--;
                    }
                }

                if (fieldData == 1) // if the ship is located here
                {
                    (*leftField)[i * 10 + j].setData(3); // mark this field as destroyed deck
                    moves.erase(check); // erase this positions from possible computer moves

                    markTheDeck(i, j, leftField); // mark corners of the deck as used
                    if (checkShipIsKilled(i, j, leftField)) // check whether this ship is destroyed or not
                    {
                        possibleDirections.clear(); // if destroyed - erase or possible directions
                        currentDirection = -1;
                    }

                    RectangleShape hitPos(Vector2f(fieldSize, fieldSize));
                    hitPos.setPosition((*leftField)[i * 10 + j].getPosition());
                    hitPos.setTexture(&computerHitTexture);

                    hitPositions.emplace_back(hitPos); // add destroyed position to vector to display
                }
            }
            else // there's no ship here
            {
                if (check != moves.end())  // if this position is in the list of possible computer positions
                {
                    moves.erase(check); // erase it

                    (*leftField)[i * 10 + j].setData(4); // set appropriate data and texture
                    (*leftField)[i * 10 + j].displayMissTexture();
                    playerMove = true; // the next turn is for player
                }

                possibleDirections.erase(std::find(possibleDirections.begin(), possibleDirections.end(), currentDirection)); // erase current direction
                currentDirection = -1;
                i = prevI; j = prevJ; // restore start position
                flag = false;
            }
        }
        else
        {
            int k = moves[rand() % moves.size()]; // choose random position
            int fieldData = (*leftField)[k].getData();

            i = k / 10; prevI = i; // get coordinates of this position and remember start position
            j = k % 10; prevJ = j;

            auto it = std::find(moves.begin(), moves.end(), k); // erase this position from the list of possible computer moves
            if (it != moves.end()) moves.erase(it);

            if (fieldData == 1) // if there's a ship here
            {
                (*leftField)[k].setData(3); // mark position as destroyed deck

                markTheDeck(i, j, leftField); // mark corners of the field

                RectangleShape hitPos(Vector2f(fieldSize, fieldSize));
                hitPos.setPosition((*leftField)[k].getPosition());
                hitPos.setTexture(&computerHitTexture);

                hitPositions.emplace_back(hitPos); // add texture to display

                if (!checkShipIsKilled(i, j, leftField)) // check whether this ship is destroyed
                {
                    std::array<int, 2> taken{3, 4};

                    // checking positions at the top, bottom, left and right of the current one
                    // if position is free - add it to list of possible directions to check orientation of the ship and destroy it later
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
            else if (fieldData == 0 || fieldData == 2) // if computer has missed
            {
                (*leftField)[k].setData(4); // set appropriate data and texture
                (*leftField)[k].displayMissTexture();
                playerMove = true; // the next turn is for player
            }
        }
    }

    return flag; // make a delay depending on who made this move
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
    if (!playerShips) 
    {
        drawSurvivedShips();
        return L"Поражение!";
    }
    return L"Победа!";
}
