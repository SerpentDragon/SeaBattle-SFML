#include "Interface.h"

Text globalTime;
int hour = 0, minute = 0, second = 0;

void timer()
{
    std::stringstream time;
    while(true)
    {
        if (hour < 10) time << 0;
        time << hour << ":";
        if (minute < 10) time << 0;
        time << minute << ":";
        if (second < 10) time << 0;
        time << second;

        globalTime.setString(time.str());
        time.str(""); 

        second++;
        if (second % 60 == 0 && second)
        {
            minute++;
            second = 0;
        }
        if (minute % 60 == 0 && minute)
        {
            hour++;
            second = minute = 0;
        }
        sleep(milliseconds(1000));
    }
}

bool comparator(const std::string& str1, const std::string str2)
{
    int str1FirstIndex = str1.find(":"), str1SecondIndex = str1.rfind(":");
    int str2FirstIndex = str2.find(":"), str2SecondIndex = str2.rfind(":");

    int str1Hours = stoi(str1.substr(0, str1FirstIndex));
    int str1Minutes = stoi(str1.substr(str1FirstIndex + 1, 2));
    int str1Seconds = stoi(str1.substr(str1SecondIndex + 1));

    int str2Hours = stoi(str2.substr(0, str2FirstIndex));
    int str2Minutes = stoi(str2.substr(str2FirstIndex + 1, 2));
    int str2Seconds = stoi(str2.substr(str2SecondIndex + 1));

    if (str1Hours == str2Hours)
    {
        if (str1Minutes == str2Minutes)
        {
            return str1Seconds < str2Seconds;
        }
        else return str1Minutes < str2Minutes;
    }
    else return str1Hours < str2Hours;
}

void Interface::drawCoordinates(int x, int y, int size) const
{
    Text coordText("", optimaFont, static_cast<double>(size) / 1.5);
    coordText.setFillColor(Color(85, 4, 29));
    int symbol, offset = (size - coordText.getGlobalBounds().width) / 4;

    for(int i = 0, symbol = 1072; i < 10; i++, symbol++)
    {
        coordText.setString(std::to_string(i + 1));

        if (i == 9) offset /= 2;

        coordText.setPosition(x + i * size + offset, y - size);
        window_->draw(coordText);

        coordText.setPosition(x + (i + 12) * size + offset, y - size);
        window_->draw(coordText);

        if (symbol == 1081) symbol++;

        coordText.setString(static_cast<wchar_t>(symbol));

        coordText.setPosition(x - size + offset / 2, y + i * size);
        window_->draw(coordText);

        coordText.setPosition(x + 11 * size + offset / 2, y + i * size);
        window_->draw(coordText);
    }
}

void Interface::showMessage(const std::wstring& msg) const
{
    int windowWidth = fieldSize * 7, windowHeight = fieldSize * 4;

    Text messageText(msg, optimaFont, 0.03704 * screenHeight);
    messageText.setFillColor(Color(85, 4, 29));

    auto messageBounds = messageText.getGlobalBounds();
    messageText.setPosition((screenWidth - messageBounds.width) / 2, 
        (screenHeight - messageBounds.height) / 2.04);
    img_["dialogBackground"].setPosition((screenWidth - windowWidth) / 2, 
        (screenHeight - windowHeight) / 2);

    window_->draw(img_["dialogBackground"]);
    window_->draw(messageText);
    window_->display();

    sleep(milliseconds(3000));
}

bool Interface::showWarning(const std::wstring& msg) const
{
    int windowWidth = fieldSize * 7, windowHeight = fieldSize * 4;
    int windowXPos = window_->getPosition().x + (window_->getSize().x - windowWidth) / 2;
    int windowYPos = window_->getPosition().y + (window_->getSize().y - windowHeight) / 2;

    RenderWindow warningWindow(VideoMode(windowWidth, windowHeight), L"Предупреждение!", Style::None);
    warningWindow.setPosition(Vector2i(windowXPos, windowYPos));

    Text messageText(msg, optimaFont, 0.03704 * screenHeight);
    messageText.setFillColor(Color(85, 4, 29));
    messageText.setPosition((windowWidth - messageText.getGlobalBounds().width) / 2, fieldSize / 2);

    Button yesButton(&warningWindow, Text(L"Да", optimaFont, 0.0157 * screenWidth), 
        fieldSize, fieldSize * 2.5, fieldSize * 2, fieldSize, 
        img_["dialogWindowButton"].getTexture(), 
        img_["dialogWindowSelectedButton"].getTexture(), 
        Color(85, 4, 29), Color(0, 36, 91));
    Button noButton(&warningWindow, Text(L"Нет", optimaFont, 0.0157 * screenWidth), 
        fieldSize * 4, fieldSize * 2.5, fieldSize * 2, fieldSize,
        img_["dialogWindowButton"].getTexture(), 
        img_["dialogWindowSelectedButton"].getTexture(), 
        Color(85, 4, 29), Color(0, 36, 91));

    img_["dialogBackground"].setPosition(0, 0);

    Event event;

    while(true)
    {
        while(warningWindow.pollEvent(event))
        {
            switch(event.type){}
        }

        warningWindow.clear();

        warningWindow.draw(img_["dialogBackground"]);

        warningWindow.draw(messageText);

        yesButton.drawButton();
        noButton.drawButton();

        if (yesButton.isPressed()) 
        {
            warningWindow.close();
            return true;
        }
        else if (noButton.isPressed())
        {
            warningWindow.close();
            return false;
        } 

        warningWindow.display();
    }
}

std::vector<std::string> Interface::readRecords() const
{
    std::vector<std::string> records;

    std::fstream file("data/records.ltx", std::ios_base::in | std::ios_base::binary);
    if (!file)
    {
        std::filesystem::create_directories("data");
        std::ifstream tmpFile("data/records.ltx");
        tmpFile.close();
    }
    else
    {
        std::string record;
        std::regex regexTemplate("[0-9]{2,}:[0-9]{2}:[0-9]{2}");

        while(std::getline(file, record))
        {
            if (std::regex_match(record, regexTemplate))
            {
                int min = stoi(record.substr(record.find(":") + 1, 2));
                int sec = stoi(record.substr(record.rfind(":") + 1));

                if (min < 59 && sec < 59) records.emplace_back(record);
            }
        }
        file.close();

        std::sort(records.begin(), records.end(), comparator);
    }

    return records;
}

void Interface::writeRecords() const
{
    std::vector<std::string> records = readRecords();
    records.emplace_back(globalTime.getString());
    std::sort(records.begin(), records.end(), comparator);

    std::fstream file("data/records.ltx", std::ios_base::out | std::ios_base::binary);
    for(int i = 0; i < 5 && i < records.size(); i++) file << records[i] << std::endl;
    file.close();
}

Interface::Interface(RenderWindow* window) : window_(window)
{
    img_.emplace("startWindow", Vector2f(Width, Height)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/StartWindowBackground"));

    img_.emplace("gameWindow", Vector2f(screenWidth, screenHeight)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/GameWindowBackground"));

    img_.emplace("referenceWindow", Vector2f(0.5 * Width, 0.6 * Height)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/ReferenceWindowBackground"));

    img_.emplace("fieldBackground", Vector2f(24 * fieldSize, 12 * fieldSize)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/FieldBackground"));
    img_["fieldBackground"].setPosition(0.1823 * screenWidth, 0.12038 * screenHeight);

    img_.emplace("recordsWindow", Vector2f(screenWidth / 5, screenHeight / 2)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/RecordsWindowBackground"));

    img_.emplace("dialogBackground", Vector2f(fieldSize * 7, fieldSize * 4)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/DialogWindowsBackground"));

    img_.emplace("mainWindowsButton", Vector2f(0.5 * Width, 0.07831 * Height)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/buttons/MainWindowsButton"));

    img_.emplace("mainWindowsSelectedButton", Vector2f(0.255 * Width, 0.07831 * Height)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/buttons/MainWindowsSelectedButton"));

    img_.emplace("dialogWindowButton", Vector2f(fieldSize * 2, fieldSize)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/buttons/DialogWindowButton"));

    img_.emplace("dialogWindowSelectedButton", Vector2f(fieldSize * 2, fieldSize)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/buttons/DialogWindowSelectedButton"));

    img_.emplace("timerBackground", Vector2f(fieldSize * 5, fieldSize * 1.3)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/TimerBackground"));
    img_["timerBackground"].setPosition(0.33 * screenWidth, 0.88 * screenHeight);
}

Interface::~Interface()
{
    window_ = nullptr;
}

void Interface::mainMenu() const
{
    int button_width = 0.23 * Width;
    int button_height = 0.08 * Height;
    int font_size = 0.053 * Height;
    int button_xPos = 0.73 * Width;

    Text button_text(L"", optimaFont, font_size);
    const std::wstring writing[] = {L"Играть", L"Справка", L"Рекорды", L"Выход"}; // main buttons of the window
    std::vector<Button> buttons;

    for(size_t i = 0; i < 4; i++) 
    {
        button_text.setString(writing[i]);
        buttons.emplace_back(Button(window_, button_text, button_xPos, 
            (0.3358 + i * 0.1092) * Height, button_width, button_height,
            img_["mainWindowsButton"].getTexture(), 
            img_["mainWindowsSelectedButton"].getTexture(), 
            Color(85, 4, 29), Color(0, 36, 91)));
        buttons[i].setTextColor(Color::Black);
    }

    Event event;   

    while(window_->isOpen())
    {
        while(window_->pollEvent(event))
        {
            switch(event.type) {}
        }

        window_->clear(Color(240, 240, 240));

        window_->draw(img_["startWindow"]);

        if (buttons[0].isPressed()) gameWindow();           // Play button
        else if (buttons[1].isPressed()) showReference();   // Reference button
        else if (buttons[2].isPressed())                    // Records button
        {
            showRecords();
        }                   
        else if (buttons[3].isPressed())                    // Exit button
        {
            if (showWarning(L"Вы уверены, что\n  хотите выйти?")) window_->close(); 
        }

        window_->display();
    }
}

void Interface::gameWindow() const
{
    int button_width = 0.1178 * screenWidth, button_height = 0.078 * screenHeight;
    int font_size = 0.025 * screenWidth;

    Button startButton(window_, Text(L"Старт", optimaFont, font_size), 
        0.0418 * screenWidth, 0.88 * screenHeight, button_width, button_height, 
        img_["mainWindowsButton"].getTexture(), 
        img_["mainWindowsSelectedButton"].getTexture(), 
        Color(85, 4, 29), Color(0, 36, 91));
    startButton.setTextColor(Color::Black);
    Button exitButton(window_, Text(L"Выход", optimaFont, font_size), 
        0.84 * screenWidth, 0.88 * screenHeight, button_width, button_height, 
        img_["mainWindowsButton"].getTexture(), 
        img_["mainWindowsSelectedButton"].getTexture(), 
        Color(85, 4, 29), Color(0, 36, 91));
    exitButton.setTextColor(Color::Black);
    Button autoButton(window_, Text(L"Авто", optimaFont, font_size), 
        0.185 * screenWidth, 0.88 * screenHeight, button_width, button_height, 
        img_["mainWindowsButton"].getTexture(), 
        img_["mainWindowsSelectedButton"].getTexture(), 
        Color(85, 4, 29), Color(0, 36, 91));
    autoButton.setTextColor(Color::Black);


    std::vector<Field> leftField, rightField; // create game fields
    for(size_t i = 0; i < 10; i++) 
    {
        for(size_t j = 0; j < 10; j++)
        {
            leftField.emplace_back(Field(window_, xCoord + i * fieldSize, yCoord + j * fieldSize));
            rightField.emplace_back(Field(window_, xCoord + (i + 12) * fieldSize, yCoord + j * fieldSize));
        }
    }

    globalTime = Text("00:00:00", optimaFont, 0.0265 * screenWidth); // text for timer
    globalTime.setFillColor(Color(85, 4, 29));
    globalTime.setPosition(0.375 * screenWidth, 0.893 * screenHeight);

    Thread th(&timer);

    std::vector<Ship> ships; // create ships
    for(int i = 3; i >= 0; i--)
    {
        for(size_t j = 0; j < i + 1; j++) 
        {
            ships.emplace_back(Ship(window_, 4 - i, 0.0261 * screenWidth, 0.3255 * screenHeight + i * fieldSize * 1.1));
        }
    }

    int movement = -1;
    int dx, dy;
    bool checkGameStarted = false, checkPause = false;
    hour = minute = second = 0;

    Mechanics mech(window_, &leftField, &rightField);

    window_->create(VideoMode(screenWidth, screenHeight), "Морской бой", Style::Fullscreen);

    Event event;

    while(window_->isOpen())
    {
        while(window_->pollEvent(event))
        {
            switch(event.type)
            {
                case Event::MouseButtonPressed:
                    if (event.mouseButton.button == Mouse::Left && !checkGameStarted) 
                    {
                        int x = Mouse::getPosition(*window_).x;
                        int y = Mouse::getPosition(*window_).y;
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
                        int x = Mouse::getPosition(*window_).x;
                        int y = Mouse::getPosition(*window_).y;
                        ships[movement].setPosition(x - dx, y - dy); // if the cursore is above the ship, move it
                        ships[movement].setFieldColor(leftField);
                    }
                    break;
                case Event::MouseButtonReleased:
                    if (event.mouseButton.button == Mouse::Left 
                        && movement != -1 && !checkGameStarted) 
                    {
                        ships[movement].placeShip(leftField); // free ship
                        movement = -1;
                        dx = dy = 0;
                    }
                    else if (event.mouseButton.button == Mouse::Right 
                        && Mouse::isButtonPressed(Mouse::Left) && !checkGameStarted)
                    {
                        int x = Mouse::getPosition(*window_).x;
                        int y = Mouse::getPosition(*window_).y;
                        for(size_t i = 0; i < 10; i++)
                        {
                            if (ships[i].onShip(x, y) && ships[i].isPlaced()) // rotate ship relatively to cursor
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

        window_->clear();

        window_->draw(img_["gameWindow"]);
        window_->draw(img_["fieldBackground"]);
        window_->draw(img_["timerBackground"]);

        exitButton.drawButton();
        startButton.drawButton();
        autoButton.drawButton();

        if (checkGameStarted && !checkPause)
        {
            if (mech.startTheGame()) sleep(milliseconds(500));
        }

        for(int i = 0; i < 100; i++) 
        {
            leftField[i].drawField();
            rightField[i].drawField();
        }

        drawCoordinates(xCoord, yCoord, fieldSize);

        for(size_t i = 0; i < 10; i++) ships[i].drawShip();

        window_->draw(globalTime);

        if (checkGameStarted) mech.drawPositions();

        if (checkGameStarted && !checkPause)
        {
            if (mech.checkEndGame())
            {
                std::wstring result = mech.getResult();
                if (result == L"Победа!") writeRecords();
                showMessage(result);
                break;
            }
        }

        if (startButton.isPressed())
        {
            bool flag = true;

            if (!checkGameStarted)
            {
                for(const auto& ship : ships) // check if all the sips were placed 
                {
                    if (ship.isPlaced())
                    {
                        flag = false;
                        showMessage(L"Корабли неверно\n\tрасставлены!");
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
                    mech.placeShips(&rightField, &ships);
                    startButton.setText(L"Пауза");
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
        else if (autoButton.isPressed() && !checkGameStarted)
        {
            for(auto& ship : ships) ship.resetPosition(leftField);
            for(auto& field : leftField) field.resetData();

            mech.placeShips(&leftField, &ships);           
        }
        else if (exitButton.isPressed())
        {
            hour = minute = second = 0;
            if (showWarning(L"Вы уверены, что\n  хотите выйти?")) break;
        }
        
        window_->display(); 

        
    }

    th.terminate();
    window_->create(VideoMode(Width, Height), "Морской бой", Style::Close);
    window_->setPosition(Vector2i((screenWidth - Width) / 2, (screenHeight - Height) / 2)); 
}

void Interface::showReference() const
{
    RenderWindow refWindow(VideoMode(0.5 * Width, 0.6 * Height), L"Справка", Style::Close);

    int windowXPos = window_->getPosition().x + (window_->getSize().x - 0.5 * Width) / 2;
    int windowYPos = window_->getPosition().y + (window_->getSize().y - 0.6 * Height) / 2;

    refWindow.setPosition(Vector2i(windowXPos, windowYPos));
    Event event;

    Text text(L"В начале игры вам необходимо расставить свои \nкорабли на левом поле:\n1 - четырехпалубный\n2 - трехпалубных\n3 - двухпалубных\n4 - однопалубных\n\
Расстановка кораблей производится путем пере-\nтаскивания их на игровое поле. Корабли не мо-\nгут касаться сторонами или углами. Пермым хо-\nдит игрок.\nВ случае \
попадания игроком по кораблю против-\nника, этот игрок продолжает свой ход. В ином \nслучае, ход переходит другому игроку. Игра за-\nканчивается, когда убиты все корабли одного \
из \nигроков (корабль считается убитым тогда, когда \nу него подбиты все палубы).", optimaFont, 0.0201 * Width);
    text.setFillColor(Color(85, 4, 29));
    text.setPosition(0.025 * Width, 0.03 * Height);

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

        refWindow.draw(img_["referenceWindow"]);

        refWindow.draw(text);

        refWindow.display();
    }
}

void Interface::showRecords() const
{
    int windowWidth = screenWidth / 5, windowHeight = screenHeight / 2;
    int windowXPos = window_->getPosition().x + (window_->getSize().x - windowWidth) / 2;
    int windowYPos = window_->getPosition().y + (window_->getSize().y - windowHeight) / 2;

    std::vector<std::string> records = readRecords();
    if (records.size() > 5) records.erase(records.begin() + 5, records.end());

    std::vector<Text> recordsTexts(records.size());
    for(int i = 0; i < records.size(); i++)
    {
        recordsTexts[i] = Text(std::to_string(i + 1) + ") " + records[i], optimaFont, 50);
        recordsTexts[i].setFillColor(Color(85, 4, 29));
        recordsTexts[i].setPosition(0.1823 * windowWidth, 
            screenHeight / 6 + i * 1.2 * recordsTexts[i].getGlobalBounds().height);
    }

    RenderWindow recordsWindow(VideoMode(windowWidth, windowHeight), L"Рекорды", Style::Close);
    recordsWindow.setPosition(Vector2i(windowXPos, windowYPos));

    Text titleText(L"Рекорды", optimaFont, 54);
    titleText.setFillColor(Color(85, 4, 29));
    titleText.setPosition((windowWidth - 
        titleText.getGlobalBounds().width) / 2, windowHeight * 0.05);

    Event event;

    while(recordsWindow.isOpen())
    {
        while(recordsWindow.pollEvent(event))
        {
            switch(event.type)
            {
                case Event::Closed:
                    recordsWindow.close();
                    break;
            }
        }

        recordsWindow.clear(Color::White);

        recordsWindow.draw(img_["recordsWindow"]);

        recordsWindow.draw(titleText);

        for(const auto& recordText : recordsTexts)
        {
            recordsWindow.draw(recordText);
        }

        recordsWindow.display();
    }
}
