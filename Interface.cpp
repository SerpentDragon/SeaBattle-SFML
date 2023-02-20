#include "Interface.h"

#include <iostream>

Text globalTime;

inline void Interface::drawCoordinates(const int& x, const int& y, const int& size) const
{
    Text coordText("", arialFont, static_cast<double>(size) / 1.5);
    int symbol, offset = (size - coordText.getGlobalBounds().width) / 4;

    for(int i = 0, symbol = 1072; i < 10; i++, symbol++)
    {
        coordText.setString(std::to_string(i + 1));

        if (i == 9) offset /= 2;

        coordText.setPosition(x + i * size + offset, y - size);
        window->draw(coordText);

        coordText.setPosition(x + (i + 12) * size + offset, y - size);
        window->draw(coordText);

        if (symbol == 1081) symbol++;

        coordText.setString(static_cast<wchar_t>(symbol));

        coordText.setPosition(x - size + offset / 2, y + i * size);
        window->draw(coordText);

        coordText.setPosition(x + 11 * size + offset / 2, y + i * size);
        window->draw(coordText);
    }
}

void timer(int& hours, int& minutes, int& seconds)
{
    std::stringstream time;
    while(true)
    {
        time << hours << ":" << minutes << ":" << seconds;
        globalTime.setString(time.str());
        time.str(""); 

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

    img["landscape"] = std::pair(Texture(), RectangleShape(Vector2f(Height, Height)));
    img["landscape"].first.loadFromFile("images/interface/landscape.jpg");
    img["landscape"].second.setTexture(&img["landscape"].first);

    img["sea"] = std::pair(Texture(), RectangleShape(Vector2f(screenWidth, screenHeight)));
    img["sea"].first.loadFromFile("images/interface/sea.jpg");
    img["sea"].second.setTexture(&img["sea"].first);
}

Interface::Interface(const Interface& obj)
{
    window = obj.window;
    img = obj.img;
}

Interface::Interface(Interface&& obj) noexcept : window(obj.window), img(obj.img)
{
    obj.window = nullptr;
}

Interface& Interface::operator=(const Interface& obj)
{
    if (this != &obj)
    {
        window = obj.window;
        img = obj.img;
    }
    return *this;
}

Interface& Interface::operator=(Interface&& obj) noexcept
{
    if (this != &obj)
    {
        window = obj.window;
        img = obj.img;

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
    Text titleText(L"Морской бой", trebuchetFont, 0.074 * Height);
    titleText.setFillColor(Color::Black);
    titleText.setPosition(0.5 * Height + (Width - titleText.getGlobalBounds().width) / 2, 0);

    int button_width = 0.168 * Width;
    int button_height = 0.077 * Height;
    int font_size = 0.0497 * Height;
    int button_xPos = 0.7512 * Width;

    Text button_text(L"", arialFont, font_size);
    const std::wstring writing[] = {L"Играть", L"Справка", L"Рекорды", L"Выход"};
    std::vector<Button> buttons;
    for(size_t i = 0; i < 4; i++) 
    {
        button_text.setString(writing[i]);
        buttons.emplace_back(Button(window, button_text, button_xPos, (0.3358 + i * 0.1092) * Height, button_width, button_height, buttonColor, buttonColorOn));
        buttons[i].setTextColor(Color::Black);
    }

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

        window->draw(img["landscape"].second);

        window->draw(titleText);

        if (buttons[0].isPressed()) 
        {
            gameWindow();           // Play button
            // std::cout << "OUT!\n";
        }
        else if (buttons[1].isPressed()) showReference();   // Reference button
        else if (buttons[2].isPressed());                   // Records button
        else if (buttons[3].isPressed()) window->close();   // Exit button

        window->display();
    }
}

void Interface::gameWindow() const
{
    int button_width = 0.1178 * screenWidth, button_height = 0.078 * screenHeight;
    int font_size = 0.0161 * screenWidth;

    Button startButton(window, Text(L"Старт", arialFont, font_size), 0.0418 * screenWidth, 0.88 * screenHeight, button_width, button_height, buttonColor, buttonColorOn);
    startButton.setTextColor(Color::Black);
    Button exitButton(window, Text(L"Выход", arialFont, font_size), 0.84 * screenWidth, 0.88 * screenHeight, button_width, button_height, buttonColor, buttonColorOn);
    exitButton.setTextColor(Color::Black);

    std::vector<Field> leftField, rightField;

    for(size_t i = 0; i < 10; i++)
    {
        for(size_t j = 0; j < 10; j++)
        {
            leftField.emplace_back(Field(window, xCoord + i * fieldSize, yCoord + j * fieldSize));
            // rightField.emplace_back(Field(window, xCoord + (i + 12) * fieldSize, yCoord + j * fieldSize));
        }
    }

    for(size_t i = 0; i < 10; i++)
    {
        for(size_t j = 0; j < 10; j++)
        {
            // leftField.emplace_back(Field(window, xCoord + i * fieldSize, yCoord + j * fieldSize));
            rightField.emplace_back(Field(window, xCoord + (i + 12) * fieldSize, yCoord + j * fieldSize));
        }
    }

    globalTime = Text("0:0:0", arialFont, 0.0365 * screenWidth);
    globalTime.setFillColor(Color::Red);
    globalTime.setPosition(0.2605 * screenWidth, 0.88 * screenHeight);

    Thread th(std::bind(&timer, 0, 0, 0));

    std::vector<Ship> ships;
    for(int i = 3; i >= 0; i--)
    {
        for(size_t j = 0; j < i + 1; j++) ships.emplace_back(Ship(window, 4 - i, 0.0261 * screenWidth, 0.3255 * screenHeight + i * fieldSize * 1.1));
    }

    int movement = -1;
    int dx, dy;

    window->create(VideoMode(screenWidth, screenHeight), "Морской бой", Style::Fullscreen);

    Event event;

    while(window->isOpen())
    {
        while(window->pollEvent(event))
        {
            switch(event.type)
            {
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        int x = Mouse::getPosition(*window).x;
                        int y = Mouse::getPosition(*window).y;
                        for(size_t i = 0; i < 10; i++)
                        {
                            if (ships[i].onShip(x, y))
                            {
                                movement = i;
                                Vector2f coord = ships[i].getPos();
                                dx = x - coord.x;
                                dy = y - coord.y;
                                break;
                            }
                        }
                    }
                    break;
                case Event::MouseMoved:
                    if (Mouse::isButtonPressed(Mouse::Left) && movement != -1)
                    {
                        int x = Mouse::getPosition(*window).x;
                        int y = Mouse::getPosition(*window).y;
                        ships[movement].setPos(x - dx, y - dy);
                        ships[movement].setFieldColor(leftField);
                    }
                    break;
                case Event::MouseButtonReleased:
                    if (event.mouseButton.button == Mouse::Left && movement != -1) 
                    {
                        ships[movement].placeShip(leftField);
                        movement = -1;
                        dx = dy = 0;
                    }
                    else if (event.mouseButton.button == Mouse::Right && Mouse::isButtonPressed(Mouse::Left))
                    {
                        int x = Mouse::getPosition(*window).x;
                        int y = Mouse::getPosition(*window).y;
                        for(size_t i = 0; i < 10; i++)
                        {
                            if (ships[i].onShip(x, y) && ships[i].getIsPlaced())
                            {
                                ships[i].rotateShip(x, y);
                                ships[i].setFieldColor(leftField);
                                Vector2f coord = ships[i].getPos();
                                dx = x - coord.x;
                                dy = y - coord.y;
                                break;
                            }
                        }
                    }
                    break;
            }
        }

        window->clear();

        window->draw(img["sea"].second);

        for(int i = 0; i < 100; i++) 
        {
            leftField[i].drawField();
            rightField[i].drawField();
        }

        drawCoordinates(xCoord, yCoord, fieldSize);

        for(size_t i = 0; i < 10; i++) ships[i].drawShip();

        if (startButton.isPressed())
        {
            if (startButton.getPressedCounter() % 2) th.launch();
            else th.terminate();
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
