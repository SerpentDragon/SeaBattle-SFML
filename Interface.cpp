#include "Interface.h"

Interface::Interface(RenderWindow* window)
{
    this->window = window;
}

Interface::Interface(const Interface& obj)
{
    window = obj.window;
}

Interface::Interface(Interface&& obj) : window(obj.window)
{
    obj.window = nullptr;
}

Interface& Interface::operator=(const Interface& obj)
{
    if (this != &obj)
    {
        window = obj.window;
    }
    return *this;
}

Interface& Interface::operator=(Interface&& obj)
{
    if (this != &obj)
    {
        window = obj.window;

        obj.window = nullptr;
    }
    return *this;
}

Interface::~Interface()
{
    window = nullptr;
}

void Interface::mainMenu()
{
    Texture texture;
    texture.loadFromFile("images/landscape.jpg");

    RectangleShape screensaver(Vector2f(Height, Height));
    screensaver.setTexture(&texture);

    Font font;
    font.loadFromFile("fonts/arial.ttf");

    Font titleFont;
    titleFont.loadFromFile("fonts/trebuchetms.ttf");

    Text titleText(L"Морской бой", titleFont, 0.074 * Height);
    titleText.setFillColor(Color::Black);
    titleText.setPosition(Height + (Width - Height - titleText.getGlobalBounds().width) / 2, 0);

    int button_width = 0.168 * Width;
    int button_height = 0.077 * Height;
    int val = 150;

    Button playButton(window, Text(L"Играть", font, 0.0497 * Height), 0.7512 * Width, 0.3358 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    playButton.setTextColor(Color::Black);
    Button referenceButton(window, Text(L"Справка", font, 0.0497 * Height), 0.7512 * Width, 0.445 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    referenceButton.setTextColor(Color::Black);
    Button recordsButton(window, Text(L"Рекорды", font, 0.0497 * Height), 0.7512 * Width, 0.5542 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    recordsButton.setTextColor(Color::Black);
    Button exitButton(window, Text(L"Выход", font, 0.0497 * Height), 0.7512 * Width, 0.6634 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    exitButton.setTextColor(Color::Black);

    Event event;

    while(window->isOpen())
    {
        while(window->pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    window->close();
                    break;
            }
        }

        window->clear(Color(240, 240, 240));

        window->draw(screensaver);

        window->draw(titleText);

        playButton.drawButton();
        referenceButton.drawButton();
        recordsButton.drawButton();
        exitButton.drawButton();

        if (playButton.isPressed());
        else if (referenceButton.isPressed());
        else if (recordsButton.isPressed());
        else if (exitButton.isPressed()) window->close();

        window->display();
    }
}