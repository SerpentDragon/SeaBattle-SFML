#include "Mechanics.hpp"

namespace directions
{
    enum : unsigned short { up = 0, right, down, left };
}

void Mechanics::markTheDeck(int i, int j, std::vector<Field>* fieldArea)
{
    for(int row = -1; row <= 1; row += 2)
    {
        // limit not to go beyond field area (row must not be equal to -1 or 10)
        int rowLimit = row == -1 ? -1 : 10; 

        for(int column = -1; column <= 1; column += 2)
        {
            // limit not to go beyond field area (column must not be equal to -1 or 10)
            int columnLimit = column == -1 ? -1 : 10;

            // check corners of the deck
            if (i + row != rowLimit && j + column != columnLimit) 
            {
                (*fieldArea)[(i + row) * 10 + j + column].displayMissTexture();
                if (fieldArea == leftField_)
                {
                    auto it = std::find(moves_.begin(), 
                        moves_.end(), (i + row) * 10 + j + column);
                    if (it != moves_.end()) moves_.erase(it);
                }
            }
        }
    }
}

void Mechanics::markKilledShip(const std::vector<int>& positions, std::vector<Field>* fieldArea)
{
    // check all chosen positions around the ship
    for(const auto& position : positions) 
    {
         // mark this field as used & display appropriate texture
        (*fieldArea)[position].setData(field_data::hit_field);
        (*fieldArea)[position].displayMissTexture();
        
        // if the field is located on player's field
        if (fieldArea == leftField_) 
        {
            auto it = std::find(moves_.begin(), 
                moves_.end(), position);
            if (it != moves_.end()) moves_.erase(it);
        }
        
    }
}

bool Mechanics::checkShipIsKilled(int i, int j, std::vector<Field>* fieldArea)
{
    // variables to check ship's orientation
    bool vertical = false, horizontal = false; 
    int row, column, *coord;
    int deckCounter = 0;

    std::vector<int> used_positions;

    for(int direct = directions::up; direct <= directions::left; direct++)
    {
        row = i; column = j;

        // move up or left: -1; down or right: +1
        int step = direct == directions::up || direct == directions::left ? -1 : 1; 
        // can not move away the area
        int limit = direct == directions::up || direct == directions::left ? -1 : 10;
        // coordinate to change
        coord = direct % 2 ? &column : &row;
        *coord += step;

        if (*coord == limit) continue;

        int fieldData = (*fieldArea)[row * 10 + column].getData(); 
        if (fieldData == field_data::taken 
            || fieldData == field_data::hit_ship)
        {
            direct % 2 ? horizontal = true : vertical = true;
            break;
        }
    }

    row = i; column = j;

    // if vertical == false and horizontal == false => this ship has the only deck
    if (!vertical && !horizontal) 
    {
        // positions around this ship must be marked as used because no one ship can be here
        if (i - 1 >= 0) used_positions.emplace_back((i - 1) * 10 + j);
        if (i + 1 <= 9) used_positions.emplace_back((i + 1) * 10 + j);
        if (j - 1 >= 0) used_positions.emplace_back(i * 10 + j - 1);
        if (j + 1 <= 9) used_positions.emplace_back(i * 10 + j + 1);
    }
    else
    {
        // choose an orientation
        coord = vertical ? &row : &column;
        // check right and left or up and down directions
        for(int step = -1; step <= 1; step += 2)
        {
            row = i; column = j;

            int limit = step == -1 ? -1 : 10;
           
            while(true)
            {
                *coord += step;
                if (*coord == limit) break;

                int fieldData = (*fieldArea)[row * 10 + column].getData();
                // if data is 1 - this ship is not destroyed yet
                if (fieldData == field_data::taken) return false; 
                else if (fieldData == field_data::hit_ship) deckCounter++;
                else // if data is not 1 or 3 - this positions must be marked as used
                {
                    used_positions.emplace_back(row * 10 + column);
                    break;
                }
            }
        }
    }

    markKilledShip(used_positions, fieldArea);

    if (fieldArea == leftField_) playerShips_--;
    else 
    {
        computerShips_--;

        for(auto it = shipList_.begin(); it != shipList_.end(); it++)
        {
            if (it->getDeckNumber() == deckCounter)
            {
                shipList_.erase(it);
                break;
            }
        }
    }

    return true;
}

void Mechanics::drawSurvivedShips() const
{
    for(const auto& ship : shipList_) ship.drawShip();
}

Mechanics::Mechanics(RenderWindow* window, 
    std::vector<Field>* leftField, std::vector<Field>* rightField)
    : window_(window), leftField_(leftField), rightField_(rightField),
    playerMove_(true), playerShips_(10), computerShips_(10)
{
    moves_.resize(100);
    for(int i = 0; i < moves_.size(); i++) moves_[i] = i;
}

Mechanics::~Mechanics()
{
    window_ = nullptr;
    leftField_ = nullptr;
    rightField_ = nullptr;
}

void Mechanics::placeShips(std::vector<Field>* fieldArea, std::vector<Ship>* ships) const
{
    bool tmp = false;
    int i, j, k;

    int decks[] = {4, 3, 3, 2, 2, 2, 1, 1, 1, 1};
    int decksSize = sizeof(decks) / sizeof(int);

    std::vector<int> direct;

    for(int count = 0; count < decksSize; count++)
    {
        while(true)
        {           
            // choose the place for a ship
            i = rand() % 10;
            j = rand() % 10;

            // if the number of ship's deck 
            // is 1 we can place it right now
            if (decks[count] == 1) 
            {
                if ((*fieldArea)[i * 10 + j].getData()) continue;

                (*fieldArea)[i * 10 + j].setData(field_data::taken);
                direct.emplace_back(directions::up);
                break;
            }

            int tmpI = i, tmpJ = j;

            for(size_t direction = directions::up; direction < directions::left; direction++)
            {
                int step; // we can move up or left (-1) and down or right (+1)
                int limit; // we cannot move out the field (cells with numbers 0 and 9)
                // depending on the direction we must shange one of variables  
                int *ptr = direction % 2 ? &tmpJ : &tmpI; 

                if (direction == directions::up || direction == directions::left) 
                    step = limit = -1;
                else
                {
                    step = 1; // move in positive direction
                    limit = 10; // cannot step on the cell number 10 - it doesn't exist!
                }

                for(k = 0; k < decks[count] && (*ptr) != limit; k++, (*ptr) += step)
                {
                    if ((*fieldArea)[tmpI * 10 + tmpJ].getData()) 
                        break; // if cell is taken - break 
                }
                // if there number of free cells equals to deck we remember current direction
                if (k == decks[count]) direct.emplace_back(direction);

                tmpI = i; tmpJ = j;
            }

            if (direct.size()) break;
        }
        
        int num = rand() % direct.size(); // random direction   

        int step = direct[num] == directions::up || direct[num] == directions::left ? -1 : 1;
        int *ptr = direct[num] % 2 ? &j : &i;

        int leftXBorder = i - 1, leftYBorder = j - 1;
        int rightXBorder = i + 1, rightYBorder = j + 1;

        if (direct[num] == directions::up) leftXBorder = i - decks[count];
        else if (direct[num] == directions::right) rightYBorder = j + decks[count];
        else if (direct[num] == directions::down) rightXBorder = i + decks[count];
        else if (direct[num] == directions::left) leftYBorder = j - decks[count];

        if (leftXBorder < 0) leftXBorder++;
        else if (rightXBorder > 9) rightXBorder--;
        if (leftYBorder < 0) leftYBorder++;
        else if (rightYBorder > 9) rightYBorder--;

        for(size_t m = leftXBorder; m <= rightXBorder; m++) // border the area where the ship will be placed
        {
            for(size_t n = leftYBorder; n <= rightYBorder; n++) 
                (*fieldArea)[m * 10 + n].setData(field_data::ship_near);
        }

        Ship *shipPtr;

        if (fieldArea == leftField_) 
        {
            shipPtr = &(*ships)[9 - count];
            shipPtr->setIsPlaced(i * 10 + j);
        }
        else
        {
            shipList_.emplace_back((*ships)[9 - count]);
            shipList_[count].setIsPlaced(-1);
            shipList_[count].resetPosition(*fieldArea);

            shipPtr = &shipList_[count];
        }

        if (direct[num] % 2) shipPtr->rotateShip(shipPtr->getPosition().x, shipPtr->getPosition().y);
        if (direct[num] == directions::up)
        {
            shipPtr->setPosition((*fieldArea)[(i - decks[count] + 1) * 10 + j].getPosition());
        }
        else if (direct[num] == directions::right || direct[num] == directions::down)
        {
            shipPtr->setPosition((*fieldArea)[i * 10 + j].getPosition());
        }
        else if (direct[num] == directions::left)
        {
            shipPtr->setPosition((*fieldArea)[i * 10 + j - decks[count] + 1].getPosition());
        }

        // place the ship
        for(k = 0; k < decks[count]; k++, (*ptr) += step) 
            (*fieldArea)[i * 10 + j].setData(field_data::taken);

        direct.clear();
    }
}

bool Mechanics::startTheGame()
{
    // check if the current move is for player or computer
    bool flag = false;

    if (playerMove_)
    {
        for(int k = 0; k < rightField_->size(); k++)
        {
            if ((*rightField_)[k].isChosen())
            {
                int fieldData = (*rightField_)[k].getData();
                if (fieldData == field_data::taken)
                {
                    (*rightField_)[k].setData(field_data::hit_ship);
                    (*rightField_)[k].displayHitTexture();

                    markTheDeck(k / 10, k % 10, rightField_);
                    checkShipIsKilled(k / 10, k % 10, rightField_);
                }
                else if (fieldData == field_data::free 
                    || fieldData == field_data::ship_near)
                {
                    (*rightField_)[k].setData(field_data::hit_field);
                    (*rightField_)[k].displayMissTexture();
                    playerMove_ = false;
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

        // if there are directions around the ship to check
        if (possibleDirections_.size())
        {
            static int currentDirection = -1;
            if (currentDirection == -1) 
                // choose random direction
                currentDirection = possibleDirections_[rand() % possibleDirections_.size()]; 

            // variable to change
            int *coord = currentDirection % 2 ? &j : &i; 
            // can move left/up: -1; right/down: +1
            int step = currentDirection == directions::up 
                || currentDirection == directions::left ? -1 : 1;
            // can't move away the area
            int limit = currentDirection == directions::up 
                || currentDirection == directions::left ? -1 : 10;

            *coord += step;
            if (*coord == limit)
            {
                // erase current direction
                possibleDirections_.erase(std::find(possibleDirections_.begin(), 
                    possibleDirections_.end(), currentDirection));
                currentDirection = -1;
                i = prevI; j = prevJ;
                return false;
            }

            auto check = std::find(moves_.begin(), moves_.end(), i * 10 + j);

            int fieldData = (*leftField_)[i * 10 + j].getData();
            if (fieldData == field_data::taken 
                || fieldData == field_data::hit_ship)
            {
                // erase opposite directions
                for(auto it = possibleDirections_.begin(); it != possibleDirections_.end(); it++)
                {
                    if (*it % 2 != currentDirection % 2)
                    {
                        possibleDirections_.erase(it);
                        it--;
                    }
                }

                if (fieldData == field_data::taken)
                {
                    (*leftField_)[i * 10 + j].setData(field_data::hit_ship);
                    moves_.erase(check);

                    markTheDeck(i, j, leftField_);
                    if (checkShipIsKilled(i, j, leftField_))
                    {
                        // if destroyed - erase or possible directions
                        possibleDirections_.clear(); 
                        currentDirection = -1;
                    }

                    RectangleShape hitPos(Vector2f(gl::fieldSize, gl::fieldSize));
                    hitPos.setPosition((*leftField_)[i * 10 + j].getPosition());
                    hitPos.setTexture(TextureManager::getManager()->getTexture(
                        "textures/marks/computerHit"));

                    hitPositions_.emplace_back(hitPos);
                }
            }
            else
            {
                if (check != moves_.end())
                {
                    moves_.erase(check);

                    (*leftField_)[i * 10 + j].setData(field_data::hit_field);
                    (*leftField_)[i * 10 + j].displayMissTexture();
                    playerMove_ = true;
                }

                possibleDirections_.erase(std::find(possibleDirections_.begin(), 
                    possibleDirections_.end(), currentDirection));
                currentDirection = -1;
                i = prevI; j = prevJ;
                flag = false;
            }
        }
        else
        {
            int k = moves_[rand() % moves_.size()]; // choose random position
            int fieldData = (*leftField_)[k].getData();

            // get coordinates of this position and remember start position
            i = k / 10; prevI = i;
            j = k % 10; prevJ = j;

            // erase this position from the list of possible computer moves
            auto it = std::find(moves_.begin(), moves_.end(), k);
            if (it != moves_.end()) moves_.erase(it);

            if (fieldData == field_data::taken)
            {
                (*leftField_)[k].setData(field_data::hit_ship);

                markTheDeck(i, j, leftField_);

                RectangleShape hitPos(Vector2f(gl::fieldSize, gl::fieldSize));
                hitPos.setPosition((*leftField_)[k].getPosition());
                hitPos.setTexture(TextureManager::getManager()->getTexture(
                    "textures/marks/computerHit"));

                hitPositions_.emplace_back(hitPos);

                if (!checkShipIsKilled(i, j, leftField_))
                {
                    std::array<int, 2> taken{3, 4};

                    // checking positions at the top, bottom, left and right of the current one
                    // if position is free - add it to list of possible directions to check 
                    // orientation of the ship and destroy it later
                    if (i - 1 >= 0) 
                    {
                        if (std::find(taken.begin(), taken.end(), 
                            (*leftField_)[(i - 1) * 10 + j].getData()) == taken.end())
                            possibleDirections_.emplace_back(directions::up);
                    }
                    if (i + 1 <= 9) 
                    {
                        if (std::find(taken.begin(), taken.end(), 
                            (*leftField_)[(i + 1) * 10 + j].getData()) == taken.end())
                            possibleDirections_.emplace_back(directions::down);
                    }
                    if (j - 1 >= 0) 
                    {
                        if (std::find(taken.begin(), taken.end(), 
                            (*leftField_)[i * 10 + j - 1].getData()) == taken.end())
                            possibleDirections_.emplace_back(directions::left);
                    }
                    if (j + 1 <= 9) 
                    {
                        if (std::find(taken.begin(), taken.end(), 
                            (*leftField_)[i * 10 + j + 1].getData()) == taken.end())
                            possibleDirections_.emplace_back(directions::right);
                    }
                }
            }
            else if (fieldData == field_data::free 
                || fieldData == field_data::ship_near)
            {
                (*leftField_)[k].setData(field_data::hit_field);
                (*leftField_)[k].displayMissTexture();
                playerMove_ = true;
            }
        }
    }

    // make a delay depending on who made this move
    return flag;
}

void Mechanics::drawPositions() const
{
    for(const auto& position : hitPositions_) 
        window_->draw(position);
}

bool Mechanics::checkEndGame() const
{
    return playerShips_ == 0 
        || computerShips_ == 0;
}

std::wstring Mechanics::getResult() const
{
    if (!playerShips_) 
    {
        drawSurvivedShips();
        return L"Поражение!";
    }
    return L"Победа!";
}
