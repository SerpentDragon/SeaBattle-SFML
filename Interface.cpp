#include "Interface.h"

#include <iostream>
#include <functional>

static Text globalTime;

inline void Interface::drawCoordinates(const int& x, const int& y, const int& size) const
{
    Text coordText("", arialFont, static_cast<double>(size) / 1.5);
    int symbol, offset = size - coordText.getGlobalBounds().width;

    for(int i = 0, symbol = 1072; i < 10; i++, symbol++)
    {
        coordText.setString(std::to_string(i + 1));

        coordText.setPosition(x + i * size + offset / 3, y - size);
        window->draw(coordText);

        coordText.setPosition(x + (i + 12) * size + offset / 3, y - size);
        window->draw(coordText);

        if (symbol == 1081)symbol++;

        coordText.setString((wchar_t)symbol);

        coordText.setPosition(x - size + offset / 2, y + i * size);
        window->draw(coordText);

        coordText.setPosition(x + 11 * size + offset / 2, y + i * size);
        window->draw(coordText);
    }
}

void timer(int& hours, int& minutes, int& seconds)
{
    while(true)
    {
        globalTime.setString(std::to_string(hours) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds));

        seconds++;
        if (seconds % 60 == 0 && seconds)
        {
            minutes++;
            seconds = 0;
        }
        if (minutes % 60 == 0 && minutes)
        {
            hours++;
            seconds = minutes = 0;
        }
        sleep(milliseconds(1000));
    }
}

Interface::Interface(const RenderWindow* window)
{
    this->window = const_cast<RenderWindow*>(window);
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

void Interface::mainMenu() const
{
    Texture texture;
    texture.loadFromFile("images/landscape.jpg");

    RectangleShape screensaver(Vector2f(Height, Height));
    screensaver.setTexture(&texture);

    Text titleText(L"Морской бой", trebuchetFont, 0.074 * Height);
    titleText.setFillColor(Color::Black);
    titleText.setPosition(Height + (Width - Height - titleText.getGlobalBounds().width) / 2, 0);

    int button_width = 0.168 * Width;
    int button_height = 0.077 * Height;
    int val = 150;

    Button playButton(window, Text(L"Играть", arialFont, 0.0497 * Height), 0.7512 * Width, 0.3358 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    playButton.setTextColor(Color::Black);
    Button referenceButton(window, Text(L"Справка", arialFont, 0.0497 * Height), 0.7512 * Width, 0.445 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    referenceButton.setTextColor(Color::Black);
    Button recordsButton(window, Text(L"Рекорды", arialFont, 0.0497 * Height), 0.7512 * Width, 0.5542 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    recordsButton.setTextColor(Color::Black);
    Button exitButton(window, Text(L"Выход", arialFont, 0.0497 * Height), 0.7512 * Width, 0.6634 * Height, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
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

        if (playButton.isPressed()) gameWindow();
        else if (referenceButton.isPressed()) showReference();
        else if (recordsButton.isPressed());
        else if (exitButton.isPressed()) window->close();

        window->display();
    }
}

void Interface::gameWindow() const
{
    int button_width = 0.1178 * screenWidth;
    int button_height = 0.078 * screenHeight;
    int font_size = 0.0161 * screenWidth;
    int val = 180;

    Button startButton(window, Text(L"Старт", arialFont, font_size), 0.0418 * screenWidth, 0.88 * screenHeight, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    startButton.setTextColor(Color::Black);
    Button exitButton(window, Text(L"Выход", arialFont, font_size), 0.84 * screenWidth, 0.88 * screenHeight, button_width, button_height, Color(val, val, val), Color(0, 191, 255));
    exitButton.setTextColor(Color::Black);

    int fieldSize = 0.034 * screenWidth;
    int xCoord = 0.126 * screenWidth;
    int yCoord = 0.206 * screenHeight;

    std::vector<Field> leftField, rightField;

    for(size_t i = 0; i < 10; i++)
    {
        for(size_t j = 0; j < 10; j++)
        {
            leftField.emplace_back(Field(window, xCoord + i * fieldSize, yCoord + j * fieldSize, fieldSize));
            rightField.emplace_back(Field(window, xCoord + (i + 12) * fieldSize, yCoord + j * fieldSize, fieldSize));
        }
    }

    Texture backgroundTexture;
    backgroundTexture.loadFromFile("images/sea.jpg");

    RectangleShape backgroundRect(Vector2f(screenWidth, screenHeight));
    backgroundRect.setTexture(&backgroundTexture);

    window->create(VideoMode(screenWidth, screenHeight), "Морской бой", Style::Fullscreen);

    Event event;

    int hours = 0, minutes = 0, seconds = 0;
    globalTime = Text("0:0:0", arialFont, 0.0365 * screenWidth);
    globalTime.setFillColor(Color::Red);
    globalTime.setPosition(0.2605 * screenWidth, 0.88 * screenHeight);

    Thread th(std::bind(&timer, hours, minutes, seconds));
    
    int counter = 0;

    while(window->isOpen())
    {
        while(window->pollEvent(event))
        {
            switch(event.type)
            {
                
            }
        }

        window->clear();

        window->draw(backgroundRect);

        for(int i = 0; i < 100; i++) 
        {
            leftField[i].drawField();
            rightField[i].drawField();
        }

        drawCoordinates(xCoord, yCoord, fieldSize);

        startButton.drawButton();
        exitButton.drawButton();

        if (startButton.isPressed())
        {
            if (counter % 2 == 0) th.launch();
            else th.terminate();
            counter++;
        }
        else if (exitButton.isPressed()) break;

        window->draw(globalTime);

        window->display();
    }

    th.terminate();
    window->create(VideoMode(Width, Height), "Морской бой", Style::Close);
    window->setPosition(Vector2i((screenWidth - Width) / 2, (screenHeight - Height) / 2)); 
}

void Interface::showReference() const
{
    RenderWindow refWindow(VideoMode(0.5 * Width, 0.6 * Height), L"Справка", Style::Default);
    refWindow.setPosition(Vector2i((screenWidth - 0.5 * Width) / 2, (screenHeight - 0.6 * Height) / 2));
    Event event;

    Text text(L"В начале игры вам необходимо расставить свои \nкорабли на левом поле:\n1 - четырёхпалубный\n2 - трёхпалубных\n3 - двухпалубных\n4 - однопалубных\n\
Расстановка кораблей производится путём пере-\nтаскивания их на игровое поле. Корабли не могут \nкасаться сторонами или углами. Пермым ходит \nигрок.\nВ случае \
попадания игроком по кораблю против-\nника, этот игрок продолжает свой ход. В ином \nслучае, ход переходит другому игроку. Игра закан-\nчивается, когда убиты все корабли одного \
из иг-\nроков (корабль считается убитым тогда, когда у \nнего подбиты все палубы).", arialFont, 0.020868 * Width);
    text.setFillColor(Color::Black);

    while(refWindow.isOpen())
    {
        while(refWindow.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed:
                    refWindow.close();
                    break;
            }
        }

        refWindow.clear(Color(240, 240, 240));

        refWindow.draw(text);

        refWindow.display();
    }
}
