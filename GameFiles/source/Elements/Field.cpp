#include "Field.hpp"

// check if the cursor is above the field
bool Field::onField(int xPos, int yPos) const
{
    return x_ <= xPos && xPos <= x_ + size_ 
        && y_ <= yPos && yPos <= y_ + size_;
}

Field::Field(std::shared_ptr<RenderWindow> window, int x, int y)
    : window_(window), x_(x), y_(y), size_(gl::fieldSize),
    dataCounter_(0), data_(field_data::free)
{
    // loading textures depending on what area the field is located
    hitTexture_ = x < gl::xCoord + 12 * gl::fieldSize ? 
        std::shared_ptr<Texture>(new Texture())
        : std::make_shared<Texture>(*TextureManager::getManager()->getTexture(
            "textures/marks/playerHit"));
        
    missTexture_ = x < gl::xCoord + 12 * gl::fieldSize ? 
        std::make_shared<Texture>(*TextureManager::getManager()->getTexture(
            "textures/marks/computerMissed")) 
        :  std::make_shared<Texture>(*TextureManager::getManager()->getTexture(
            "textures/marks/playerMissed"));

    field_ = RectangleShape(Vector2f(size_, size_));

    field_.setOutlineThickness(0.0019 * gl::screenHeight);
    field_.setOutlineColor(Color::Black);
    field_.setFillColor(Color::White);
    field_.setPosition(x_, y_);
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
        if (--dataCounter_ == 0) data_ = field_data::free;
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
    field_.setTexture(&(*hitTexture_));
}

void Field::displayMissTexture() 
{ 
    field_.setTexture(&(*missTexture_)); 
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

void Field::reinitField()
{
    // field_ = RectangleShape(Vector2f(size_, size_));

    // field_.setOutlineThickness(FI::fieldOutlineThickness);
    // field_.setOutlineColor(Color::Black);
    // field_.setFillColor(Color::White);
    // field_.setPosition(x_, y_);

    field_.setTexture(nullptr);

    dataCounter_ = 0;
    data_ = field_data::free;
}
