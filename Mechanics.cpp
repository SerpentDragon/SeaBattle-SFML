#include "Mechanics.h"

enum directions{up = 0, right, down, left};

void Mechanics::markTheDeck(int i, int j, std::vector<Field>* fieldArea) // should be optimized
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

void Mechanics::markKilledShip(int i, int j, std::vector<Field>* fieldArea, bool vertical, int deckNumber)
{

}

bool Mechanics::checkShipIsKilled(int i, int j, std::vector<Field>* fieldArea)
{

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
