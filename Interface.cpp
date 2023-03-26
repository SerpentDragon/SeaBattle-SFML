#include "Interface.h"

#include <iostream>

Text globalTime;

inline void Interface::drawCoordinates(int x, int y, int size) const
{
    Text coordText("", arialFont, static_cast<double>(size) / 1.5); // text for field's coordinates
    int symbol, offset = (size - coordText.getGlobalBounds().width) / 4;

    for(int i = 0, symbol = 1072; i < 10; i++, symbol++)
    {
        coordText.setString(std::to_string(i + 1)); // numbers

        if (i == 9) offset /= 2;

        coordText.setPosition(x + i * size + offset, y - size); // place numbers next to both fields
        window->draw(coordText);

        coordText.setPosition(x + (i + 12) * size + offset, y - size);
        window->draw(coordText);

        if (symbol == 1081) symbol++;

        coordText.setString(static_cast<wchar_t>(symbol)); // letters

        coordText.setPosition(x - size + offset / 2, y + i * size); // place letters next to both fields
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

    img["landscape"] = std::pair(Texture(), RectangleShape(Vector2f(Height, Height))); // load textures for window
    img["landscape"].first.loadFromFile("images/interface/landscape.jpg");
    img["landscape"].second.setTexture(&img["landscape"].first);

    img["sea"] = std::pair(Texture(), RectangleShape(Vector2f(screenWidth, screenHeight)));
    img["sea"].first.loadFromFile("images/interface/sea.jpg");
    img["sea"].second.setTexture(&img["sea"].first);
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
    const std::wstring writing[] = {L"Играть", L"Справка", L"Рекорды", L"Выход"}; // main buttons of the window
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

        if (buttons[0].isPressed()) gameWindow();           // Play button
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

    std::vector<Field> leftField, rightField; // create game fields
    for(size_t i = 0; i < 10; i++) 
    {
        for(size_t j = 0; j < 10; j++)
        {
            leftField.emplace_back(Field(window, xCoord + i * fieldSize, yCoord + j * fieldSize));
            rightField.emplace_back(Field(window, xCoord + (i + 12) * fieldSize, yCoord + j * fieldSize));
        }
    }

    globalTime = Text("0:0:0", arialFont, 0.0365 * screenWidth); // text for timer
    globalTime.setFillColor(Color::Red);
    globalTime.setPosition(0.2605 * screenWidth, 0.88 * screenHeight);

    Thread th(std::bind(&timer, 0, 0, 0)); // thread for timer

    std::vector<Ship> ships; // create ships
    for(int i = 3; i >= 0; i--)
    {
        for(size_t j = 0; j < i + 1; j++) ships.emplace_back(Ship(window, 4 - i, 0.0261 * screenWidth, 0.3255 * screenHeight + i * fieldSize * 1.1));
    }

    int movement = -1;
    int dx, dy;
    bool checkGameStarted = false, checkPause = false;

    Mechanics mech(window, &leftField, &rightField);

    window->create(VideoMode(screenWidth, screenHeight), "Морской бой", Style::Fullscreen);

    Event event;

    while(window->isOpen())
    {
        while(window->pollEvent(event))
        {
            switch(event.type)
            {
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && !checkGameStarted) // СДЕЛАТЬ ЗАПРЕТ НА ПЕРЕСТАНОВКИ ПОСЛЕ НАЧАЛА ИГРЫ!!!
                    {
                        int x = Mouse::getPosition(*window).x;
                        int y = Mouse::getPosition(*window).y;
                        for(size_t i = 0; i < 10; i++)
                        {
                            if (ships[i].onShip(x, y)) // check if the cursor is above teh ship
                            {
                                movement = i;
                                Vector2f coord = ships[i].getPosition();
                                dx = x - coord.x; // if "yes" remember current position
                                dy = y - coord.y;
                                break;
                            }
                        }
                    }
                    break;
                case Event::MouseMoved:
                    if (Mouse::isButtonPressed(Mouse::Left) && movement != -1 && !checkGameStarted)
                    {
                        int x = Mouse::getPosition(*window).x;
                        int y = Mouse::getPosition(*window).y;
                        ships[movement].setPosition(x - dx, y - dy); // if the cursore is above the ship, move it
                        ships[movement].setFieldColor(leftField);
                    }
                    break;
                case Event::MouseButtonReleased:
                    if (event.mouseButton.button == Mouse::Left && movement != -1 && !checkGameStarted) 
                    {
                        ships[movement].placeShip(leftField); // free ship
                        movement = -1;
                        dx = dy = 0;
                    }
                    else if (event.mouseButton.button == Mouse::Right && Mouse::isButtonPressed(Mouse::Left) && !checkGameStarted)
                    {
                        int x = Mouse::getPosition(*window).x;
                        int y = Mouse::getPosition(*window).y;
                        for(size_t i = 0; i < 10; i++)
                        {
                            if (ships[i].onShip(x, y) && ships[i].getIsPlaced()) // rotate ship relatively to cursor
                            {
                                ships[i].rotateShip(x, y);
                                ships[i].setFieldColor(leftField);
                                Vector2f coord = ships[i].getPosition();
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

        exitButton.drawButton();
        startButton.drawButton();

        if (checkGameStarted && !checkPause)
        {
            if (mech.startTheGame()) sleep(milliseconds(500));
        }

        for(int i = 0; i < 100; i++) 
        {
            leftField[i].drawField();
            rightField[i].drawField();
        }

        drawCoordinates(xCoord, yCoord, fieldSize); // draw coordinates of the field's cells

        for(size_t i = 0; i < 10; i++) ships[i].drawShip(); // draw ships

        window->draw(globalTime); // draw timer

        if (checkGameStarted) mech.drawPositions(); // draw hit positions

        if (checkGameStarted && !checkPause)
        {
            if (mech.checkEndGame())
            {
                showMessage(mech.getResult());
                break;
            }
        }

        if (exitButton.isPressed()) break;
        else if (startButton.isPressed())
        {
            bool flag = true;

            if (!checkGameStarted)
            {
                for(const auto& ship : ships) // check if all the sips were placed 
                {
                    if (ship.getIsPlaced())
                    {
                        flag = false;
                        showMessage(L"Корабли неверно\n\tрасставлены!");//, mech, leftField, rightField);
                        break;
                    }
                }
            }

            if (flag) // if they were, start the game
            {
                if (!checkGameStarted) 
                {
                    th.launch();
                    checkGameStarted = true;
                    mech.placeComputerShips();
                    // for(int i = 0; i < 10; i++)
                    // {
                    //     for(int j = 0; j < 10; j++)
                    //     {
                    //         std::cout << rightField[j * 10 + i].getData() << " ";
                    //     }
                    //     std::cout << std::endl;
                    // }
                }
                else
                {
                    if (startButton.getPressedCounter() % 2 == 0) 
                    {
                        th.terminate();
                        checkPause = true;
                        startButton.setText(L"Продолжить");
                    }
                    else 
                    {
                        th.launch();
                        checkPause = false;
                        startButton.setText (L"Пауза");
                    }
                }         
            }
            else // else reset ships positions
            {
                for(auto& ship : ships) ship.resetPosition(leftField);
            }
        }
        
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

void Interface::showMessage(const wchar_t* msg) const
{
    int windowWidth = fieldSize * 7, windowHeight = fieldSize * 3;

    RectangleShape messageWindow(Vector2f(windowWidth, windowHeight));
    messageWindow.setPosition((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);
    messageWindow.setOutlineThickness(0.0047 * screenHeight);
    messageWindow.setOutlineColor(Color::Black);

    Text messageText(msg, arialFont, 0.03704 * screenHeight);
    messageText.setFillColor(Color::Black);
    messageText.setPosition((screenWidth - messageText.getGlobalBounds().width) / 2, (screenHeight - messageText.getGlobalBounds().height) / 2);

    window->draw(messageWindow);
    window->draw(messageText);
    window->display();

    sleep(milliseconds(3000));
}
