#include "Ship.h"

void Ship::swap(const Ship& other) noexcept
{
    window_ = other.window_;
    deckNumber_ = other.deckNumber_;
    x_ = other.x_;
    y_ = other.y_;
    width_ = other.width_;
    height_ = other.height_;
    IsPlaced_ = other.IsPlaced_;
    rotateCounter_ = other.rotateCounter_;
    shipTexture_ = other.shipTexture_;
    shipRect_ = other.shipRect_;
    shipRect_.setTexture(shipTexture_);
    curr_x_ = other.curr_x_;
    curr_y_ = other.curr_y_;
}

bool Ship::checkCells(const std::vector<Field>& leftField, int currIndex) const
{
    int step = width_ > height_ ? 10 : 1;

    // check if all the cells from the range are free
    for(size_t i = 0; i < deckNumber_; i++, currIndex += step)
    {
        if (leftField[currIndex].getData()) return false;
    }
    return true;
}

void Ship::locateShip(std::vector<Field>& leftField, int data)
{
    int shipPlace, aroundShip;
    int xIndex, yIndex;
    int i, j, step;
    
    if (!data) 
    {
        shipPlace = aroundShip = 0; // id not data we should free cells
        xIndex = IsPlaced_ / 10;
        yIndex = IsPlaced_ % 10;
        IsPlaced_ = -1;
    }
    else 
    {
        shipPlace = 1; aroundShip = 2; // otherwise we should mark them as taken
        xIndex = (curr_x_ - xCoord) / fieldSize;
        yIndex = (curr_y_ - yCoord) / fieldSize;
        IsPlaced_ = xIndex * 10 + yIndex;
    }

    int leftXBorder = xIndex - 1;
    // calculate positions of the left upper and lower right corners of 
    int rightXBorder = width_ > height_ ? xIndex + deckNumber_ : xIndex + 1;

    int upYBorder = yIndex - 1;
    // the area around the ship
    int downYBorder = width_ > height_ ? yIndex + 1 : yIndex + deckNumber_;

    // correct borders if they are out of fields 
    // (it means that coordinates are less then 0 or greater then 9)
    if (leftXBorder < 0) leftXBorder++; 
    else if (rightXBorder > 9) rightXBorder--;
    if (upYBorder < 0) upYBorder++;
    else if (downYBorder > 9) downYBorder--;

    // mark area around the ship as taken
    for(i = leftXBorder; i <= rightXBorder; i++)
    {
        for(j = upYBorder; j <= downYBorder; j++) 
            leftField[i * 10 + j].setData(aroundShip);
    }

    step = width_ > height_ ? 10 : 1;
    for(i = xIndex * 10 + yIndex, j = 0; j < deckNumber_; j++, i += step) 
        leftField[i].setData(shipPlace); // mark cells as taken for a ship
}

int Ship::calculatePosition() const
{
    // number of the row where the ship is located
    int xIndex = (curr_x_ - xCoord) / fieldSize;
    // number of the column where the ship is located
    int yIndex = (curr_y_ - yCoord) / fieldSize; 

    // x- and y-offset relative to the upper left 
    // corner of the cell where the upper left corner 
    // of the ship is located
    int xOffset = (curr_x_ - xCoord) % fieldSize; 
    int yOffset = (curr_y_ - yCoord) % fieldSize; 

    if (xOffset > fieldSize - xOffset) xIndex++;
    if (yOffset > fieldSize - yOffset) yIndex++;

    return xIndex * 10 + yIndex;
}

Ship::Ship(RenderWindow* window, int deck, int xPos, int yPos)
    : window_(window), deckNumber_(deck), x_(xPos), curr_x_(xPos),
    y_(yPos), curr_y_(yPos), width_(deck * 0.034 * screenWidth),
    height_(0.034 * screenWidth), IsPlaced_(-1), rotateCounter_(0)
{
    shipTexture_ = *TextureManager::getManager()->getTexture(
        "textures/ships/" + std::to_string(deck) + "deck");

    shipRect_.setTexture(shipTexture_);
    shipRect_.setScale(static_cast<double>(width_) / shipTexture_.getSize().x, 
        static_cast<double>(height_) / shipTexture_.getSize().y);
    shipRect_.setOrigin(shipRect_.getLocalBounds().width / 2, 
        shipRect_.getLocalBounds().height / 2);
    shipRect_.setPosition(x_ + width_ / 2, y_ + height_ / 2);
}

Ship::Ship(const Ship& obj)
{
    swap(obj);
}

Ship::Ship(Ship&& obj) noexcept 
{
    swap(obj);    

    obj.window_ = nullptr;
    obj.deckNumber_ = obj.x_ = obj.y_ = obj.width_ = 0;
    obj.height_ = obj.curr_x_ = obj.curr_y_ = obj.rotateCounter_ = 0;
    obj.IsPlaced_ = false;
}

Ship& Ship::operator=(const Ship& obj)
{
    if (this != &obj)
    {
        swap(obj);
    }
    return *this;
}

Ship& Ship::operator=(Ship&& obj) noexcept 
{
    if (this != &obj)
    {
        swap(obj);

        obj.window_ = nullptr;
        obj.deckNumber_ = obj.x_ = obj.y_ = obj.width_ = 0;
        obj.height_ = obj.curr_x_ = obj.curr_y_ = obj.rotateCounter_ = 0;
        obj.IsPlaced_ = false;
    }
    return *this;
}

Ship::~Ship()
{
    window_ = nullptr;
}

void Ship::drawShip() const
{
    window_->draw(shipRect_);
}

bool Ship::onShip(int xPos, int yPos) const
{
    return shipRect_.getGlobalBounds().contains(xPos, yPos);
}

void Ship::rotateShip(int x, int y)
{
    ++rotateCounter_ %= 4;

    int dx = x - curr_x_;
    int dy = y - curr_y_;

    std::swap(width_, height_);

    curr_x_ = x - width_ + dy; curr_y_ = y - dx;

    shipRect_.rotate(90);
    shipRect_.setPosition(x - width_ / 2 + dy, y + height_ / 2 - dx); 
}

void Ship::setFieldColor(std::vector<Field>& leftField)
{
    static int prevIndex = 0, prevWidth = width_, prevHeight = height_;
    int step, currIndex = calculatePosition();

    // check if the ship is placed on the field
    if (IsPlaced_ != -1) locateShip(leftField, 0);

    // check if the ship is inside the user field
    if (xCoord <= curr_x_ && yCoord <= curr_y_ && 
        xCoord + 10 * fieldSize >= curr_x_ + width_ && 
        yCoord + 10 * fieldSize >= curr_y_ + height_)
    {
        // calculate orientation of the ship
        step = prevWidth > prevHeight ? 10 : 1;
        for(size_t i = 0; i < deckNumber_; i++, prevIndex += step) 
            leftField[prevIndex].setNeutralColor();
        
        // if all the cells under the ship are free, we should paint them green, else - red
        int currColor = checkCells(leftField,currIndex) ? 1 : -1;
        step = width_ > height_ ? 10 : 1;    
        for(size_t i = 0; i < deckNumber_; i++, currIndex += step) 
            leftField[currIndex].setCurrentColor(currColor);
        currIndex -= step * deckNumber_;

        prevIndex = currIndex;
        prevWidth = width_; prevHeight = height_;
    }
    else 
    {
        step = prevWidth > prevHeight ? 10 : 1;
        for(size_t i = 0; i < deckNumber_; i++, prevIndex += step) 
            leftField[prevIndex].setNeutralColor();
        prevIndex = 0;
    }
}

void Ship::placeShip(std::vector<Field>& leftField)
{
    int step = width_ > height_ ? 10 : 1;
    int currIndex = calculatePosition();

    if (xCoord <= curr_x_ && yCoord <= curr_y_ && 
        xCoord + 10 * fieldSize >= curr_x_ + width_ && 
        yCoord + 10 * fieldSize >= curr_y_ + height_)
    {
        for(size_t i = 0; i < deckNumber_; i++, currIndex += step) 
            leftField[currIndex].setNeutralColor();
        currIndex -= step * deckNumber_;

        // if all the cells under the ship are free
        if (checkCells(leftField, currIndex))
        {
            // place ship on the field
            setPosition(leftField[currIndex].getPosition());
            locateShip(leftField, 1);
        }
        // otherwise set to the ship it's starting position
        else resetPosition(leftField);
    }
    else resetPosition(leftField);
}

void Ship::resetPosition(std::vector<Field>& leftField) 
{
    // if the ship is placed we should free area around it
    if (IsPlaced_ != -1)
    {
        locateShip(leftField, 0);
        IsPlaced_ = -1;
    }
    
    int tmpCounter = 4 - rotateCounter_;
    // rotate ship until it takes it's starting position
    while(tmpCounter--) rotateShip(x_, y_);
    setPosition(x_, y_);
}

const int Ship::getDeckNumber() const { return deckNumber_; }

const Vector2f Ship::getPosition() const { return Vector2f(curr_x_, curr_y_); }

const bool Ship::isPlaced() const { return IsPlaced_ == -1; }

void Ship::setIsPlaced(int position) { IsPlaced_ = position; }

const int Ship::getWidth() const { return width_; }

const int Ship::getHeight() const { return height_; }

void Ship::setPosition(const Vector2f& vec) 
{
    curr_x_ = vec.x; curr_y_ = vec.y;
    shipRect_.setPosition(curr_x_ + width_ / 2, curr_y_ + height_ / 2);
}

void Ship::setPosition(int xPos, int yPos) 
{
    curr_x_ = xPos; curr_y_ = yPos;
    shipRect_.setPosition(xPos + width_ / 2, yPos + height_ / 2);
}
