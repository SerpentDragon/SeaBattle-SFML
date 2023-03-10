#include "Mechanics.h"
#include <iostream>

enum directions{up = 0, right, down, left};

void Mechanics::markTheDeck(int i, int j, std::vector<Field>* fieldArea) // should be optimized
{
    if (i - 1 >= 0)
    {
        if (j - 1 >= 0) 
        {
            (*fieldArea)[(i - 1) * 10 + j - 1].displayMissTexture();
            if (fieldArea == leftField) moves.emplace_back((i - 1) * 10 + j - 1);
        }
        if (j + 1 <= 9) 
        {
            (*fieldArea)[(i - 1) * 10 + j + 1].displayMissTexture();
            if (fieldArea == leftField) moves.emplace_back((i - 1) * 10 + j + 1);
        }
    }
    if (i + 1 <= 9)
    {
        if (j - 1 >= 0) 
        {
            (*fieldArea)[(i + 1) * 10 + j - 1].displayMissTexture();
            if (fieldArea == leftField) moves.emplace_back((i + 1) * 10 + j - 1);
        }
        if (j + 1 <= 9) 
        {
            (*fieldArea)[(i + 1) * 10 + j + 1].displayMissTexture();
            if (fieldArea == leftField) moves.emplace_back((i + 1) * 10 + j + 1);
        }
    }
}

bool Mechanics::checkShipIsKilled(int i, int j, std::vector<Field>* fieldArea)
{
    int row = i;
    int column = j;

    for(int direction = up; direction <= left; direction++) // go through all directions
    {
        int limit = direction == up || direction == left ? -1 : 10; // we cannot move out the field (cells with numbers 0 and 9)
        int orientation = direction == up || direction == left ? -1 : 1; // we can move up or left (-1) and down or right (+1)
        int *coord = direction == up || direction == down ? &row : &column; // choose variable to change depending on direction (vertical or horizontal)

        for(int k = 1; k < 4; k++)
        {
            if ((*coord) + orientation * k != limit) // if this position is inside the area
            {
                int tmp = *coord;
                *coord += orientation * k; 

                int fieldData = (*fieldArea)[row * 10 + column].getData();
                if (fieldData == 1) return false; // if the cell is still taken for a ship then this one wasn't killed but injured
                else if (fieldData == 2) break; // if we meet 2 then we are out of the ship placement

                *coord = tmp;
            }
        }

        row = i; column = j;
    }

    return true; // if all the cells of the ship were hit (and no one cell equals 1 what means this cell is taken for a ship) return true - the ship is killed
}

Mechanics::Mechanics(const RenderWindow *window, const std::vector<Field>* leftField, const std::vector<Field>* rightField)
{
    this->window = const_cast<RenderWindow*>(window);
    this->leftField = const_cast<std::vector<Field>*>(leftField);
    this->rightField = const_cast<std::vector<Field>*>(rightField);

    playerMove = true;
    playerShips = computerShips = 10;
}

Mechanics::~Mechanics()
{
    window = nullptr;
}

void Mechanics::placeComputerShips() const
{
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

    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            std::cout << (*rightField)[j * 10 + i].getData() << " ";
        }
        std::cout << std::endl;
    }
}

void Mechanics::startTheGame()
{
    if (playerMove)
    {
        for(int i = 0; i < 100; i++)
        {
            if ((*rightField)[i].isChosen())
            {
                int fieldData = (*rightField)[i].getData();

                if (fieldData == 1)
                {
                    (*rightField)[i].displayHitTexture(); // if the ship is located here
                    markTheDeck(i / 10, i % 10, rightField);
                    (*rightField)[i].setData(3);
                    std::cout << (checkShipIsKilled(i / 10, i % 10, rightField) ? "Killed\n" : "Injured\n");
                }
                else if (fieldData == 0 || fieldData == 2) // otherwise
                {
                    (*rightField)[i].displayMissTexture();
                    playerMove = false;
                }
                break;
            }
        }
    }
    else
    {
        int i, j;

        while(true) // THINK ABOUT IT
        {
            i = rand() % 10; // choose random position to attack
            j = rand() % 10;

            bool checkCoordinates = true;
            for(const auto& position : moves)
            {
                if (position == i * 10 + j)
                {
                    checkCoordinates = false;
                    break;
                }
            }

            if (checkCoordinates) break;
        }

        moves.emplace_back(i * 10 + j);

        int fieldData = (*leftField)[i * 10 + j].getData();
        if (fieldData == 1)
        {
            (*leftField)[i * 10 + j].setData(3);

            RectangleShape shipsDeck(Vector2f(fieldSize, fieldSize));
            shipsDeck.setTexture(&computerHitTexture);
            shipsDeck.setPosition((*leftField)[i * 10 + j].getPosition());

            hitPositions.emplace_back(shipsDeck);
            // markTheDeck(i, j, leftField);
        }
        else if (fieldData == 0 || fieldData == 2)
        {
            (*leftField)[i * 10 + j].displayMissTexture();
            playerMove = true;
        }       
    }

    for(const auto& position : hitPositions) window->draw(position);

}
