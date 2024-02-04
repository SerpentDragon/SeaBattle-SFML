#include "Field.hpp"

// check if the cursor is above the field
bool Field::onField(int xPos, int yPos) const
{
    return x_ <= xPos && xPos <= x_ + size_ 
        && y_ <= yPos && yPos <= y_ + size_;
}

void Field::swap(const Field& other) noexcept
{
    window_ = other.window_;
    field_ = other.field_;
    x_ = other.x_;
    y_ = other.y_;
    size_ = other.size_;
    data_ = other.data_;
    dataCounter_ = other.dataCounter_;
    hitTexture_ = other.hitTexture_;
    missTexture_ = other.missTexture_;
}

Field::Field(RenderWindow* window, int x, int y)
    : window_(window), x_(x), y_(y), size_(gl::fieldSize),
    dataCounter_(0), data_(field_data::free)
{
    // loading textures depending on what area the field is located
    hitTexture_ = x < gl::xCoord + 12 * gl::fieldSize ? 
        Texture() 
        : *TextureManager::getManager()->getTexture("textures/marks/playerHit"); 
    missTexture_ = x < gl::xCoord + 12 * gl::fieldSize ? 
        *TextureManager::getManager()->getTexture("textures/marks/computerMissed") 
        : *TextureManager::getManager()->getTexture("textures/marks/playerMissed");

    field_ = RectangleShape(Vector2f(size_, size_));

    field_.setOutlineThickness(FI::fieldOutlineThickness);
    field_.setOutlineColor(Color::Black);
    field_.setFillColor(Color::White);
    field_.setPosition(x_, y_);
}

Field::Field(const Field& other)
{
    swap(other);
}

Field::Field(Field&& other) noexcept
{
    swap(other);

    other.window_ = nullptr;
    other.x_ = other.y_ = other.size_ = 
        other.dataCounter_ = 0;
    other.data_ = field_data::free;
}

Field& Field::operator=(const Field& other)
{
    if (this != &other)
    {
        swap(other);
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept
{
    if (this != &other)
    {
        swap(other);

        other.window_ = nullptr;
        other.x_ = other.y_ = other.size_ = 
            other.dataCounter_ = 0;
        other.data_ = field_data::free;
    }
    return *this;
}

Field::~Field()
{
    window_ = nullptr;
}

void Field::reinitField()
{
    field_ = RectangleShape(Vector2f(size_, size_));

    field_.setOutlineThickness(FI::fieldOutlineThickness);
    field_.setOutlineColor(Color::Black);
    field_.setFillColor(Color::White);
    field_.setPosition(x_, y_);

    dataCounter_ = 0;
    data_ = field_data::free;    
}

void Field::drawField() const
{
    window_->draw(field_);
}

void Field::resetData()
{
    dataCounter_ = 0;
    data_ = field_data::free;
}

const Vector2f Field::getPosition() const { return Vector2f(x_, y_); }

const int Field::getData() const { return data_; }

void Field::setData(int data)
{
    if (data == 0) 
    {
        if (--dataCounter_ == 0) 
            data_ = field_data::free;
    }
    else
    {
        dataCounter_++;
        data_ = data;
    }
}

void Field::setCorrectColor() 
{ 
    field_.setFillColor(Color::Green); 
}

void Field::setWrongColor() 
{ 
    field_.setFillColor(Color::Red); 
}

void Field::setNeutralColor() 
{ 
    field_.setFillColor(Color::White); 
}

void Field::setCurrentColor(int valid)
{
    if (valid == -1) setWrongColor();
    else if (valid == 0) setNeutralColor();
    else if (valid == 1) setCorrectColor();
}

void Field::displayHitTexture() 
{
    field_.setTexture(&hitTexture_);
}

void Field::displayMissTexture() 
{ 
    field_.setTexture(&missTexture_); 
}

bool Field::isChosen() const
{
    int x = Mouse::getPosition(*window_).x;
    int y = Mouse::getPosition(*window_).y;
    
    if (onField(x, y))
    {
        if (Mouse::isButtonPressed(Mouse::Left))
        {
            while(true) // while left mouse button is held
            {
                if (!Mouse::isButtonPressed(Mouse::Left)) 
                    break;
            }
            if (onField(Mouse::getPosition(*window_).x, Mouse::getPosition(*window_).y)) 
                return true; // the field was chosen
        }
    }

    return false;
}
