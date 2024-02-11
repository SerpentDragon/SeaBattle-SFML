#include "Interface.hpp"

#include <iostream>

void Interface::drawCoordinates(int x, int y, int size) const
{
    int length = coordinates_.size();
    int offset = (size - coordinates_[0].getGlobalBounds().width) / 4;

    for(int i = 0; i < length; i++)
    {
        i < length / 2 ? 
            coordinates_[i].setPosition(x + i * size + offset, y - size) : 
            coordinates_[i].setPosition(x - size + offset / 2, 
                y + (i - length / 2) * size);
        window_->draw(coordinates_[i]);
    }

    for(int i = 0; i < length; i++)
    {
        i < length / 2 ? 
            coordinates_[i].setPosition(x + (i + 12) * size + offset, y - size) : 
            coordinates_[i].setPosition(x + 11 * size + offset / 2, 
                y + (i - length / 2) * size);
        window_->draw(coordinates_[i]);
    }
}

void Interface::reinitGameParams()
{
    timer_ = Timer();
    gameplay_ = Gameplay(window_, &leftField_, &rightField_);

    for(size_t i = 0; i < leftField_.size(); i++)
    {
        leftField_[i].reinitField();
        rightField_[i].reinitField();
    }

    for(auto& ship : ships_) ship.resetPosition(leftField_);

    startButton_.setText(Localizer::translate(L"Старт"));
}

std::vector<Text> Interface::createMessageBoxText(
    const std::vector<std::wstring>& msg) const
{
    std::vector<Text> messageText;
    for(int i = 0; i < msg.size(); i++)
    {
        messageText.emplace_back(Text(msg[i], gl::optimaFont, 
            IN::infoWindowTextFontSize));
        messageText[i].setFillColor(IN::textColor);
    }

    return messageText;
}

void Interface::showMessage(const std::vector<std::wstring>& msg) const
{
    auto messageText = createMessageBoxText(msg);

    for(int i = 0; i < messageText.size(); i++)
    {
        auto messageBounds = messageText[i].getGlobalBounds();
        messageText[i].setPosition((gl::screenWidth - messageBounds.width) / 2, 
            (gl::screenHeight - messageBounds.height) / 2.2 + 
            i * messageBounds.height * 1.2);
    }

    img_["dialogBackground"].setPosition((gl::screenWidth - IN::infoWindowWidth) / 2, 
        (gl::screenHeight - IN::infoWindowHeight) / 2);

    window_->draw(img_["dialogBackground"]);
    for(const auto& str : messageText) window_->draw(str);
    window_->display();

    sleep(milliseconds(3000));
}

bool Interface::showWarning(const std::vector<std::wstring>& msg) const
{
    int windowXPos = window_->getPosition().x + 
        (window_->getSize().x - IN::infoWindowWidth) / 2;
    int windowYPos = window_->getPosition().y + 
        (window_->getSize().y - IN::infoWindowHeight) / 2;

    auto warningWindow = std::make_shared<RenderWindow>(VideoMode(IN::infoWindowWidth, 
        IN::infoWindowHeight), Localizer::translate(L"Предупреждение!"), Style::None);
    warningWindow->setPosition(Vector2i(windowXPos, windowYPos));

    auto messageText = createMessageBoxText(msg);

    for(int i = 0; i < messageText.size(); i++)
    {
        messageText[i].setPosition((IN::infoWindowWidth - 
            messageText[i].getGlobalBounds().width) / 2, 
            gl::fieldSize / 2 + i * messageText[i].getGlobalBounds().height * 1.5);
    }

    auto texture = std::make_shared<Texture>(*TextureManager::getManager()->
        getTexture("textures/interface/buttons/dialog"));
    auto textureOn = std::make_shared<Texture>(*TextureManager::getManager()->
        getTexture("textures/interface/buttons/dialogSelected"));

    Button yesButton(warningWindow, Text(Localizer::translate(L"Да"), gl::optimaFont, 
        IN::infoWindowButtonFontSize), IN::infoWindowYesButtonXPos, IN::infoWindowButtonYPos, 
        IN::infoWindowButtonWidth, IN::infoWindowButtonHeight, texture, textureOn, 
        IN::textColor, IN::textColorOn);
    Button noButton(warningWindow, Text(Localizer::translate(L"Нет"), gl::optimaFont, 
        IN::infoWindowButtonFontSize), IN::infoWindowNoButtonXPos, IN::infoWindowButtonYPos, 
        IN::infoWindowButtonWidth, IN::infoWindowButtonHeight, texture, textureOn, 
        IN::textColor, IN::textColorOn);

    img_["dialogBackground"].setPosition(0, 0);

    Event event;

    while(true)
    {
        while(warningWindow->pollEvent(event))
        {
            switch(event.type) {}
        }

        warningWindow->clear();

        warningWindow->draw(img_["dialogBackground"]);

        for(const auto& str : messageText) warningWindow->draw(str);

        yesButton.drawButton();
        noButton.drawButton();

        if (yesButton.isPressed()) 
        {
            warningWindow->close();
            return true;
        }
        else if (noButton.isPressed())
        {
            warningWindow->close();
            return false;
        } 

        warningWindow->display();
    }
}

void Interface::recreateTexts()
{
    playButton_.setText(Localizer::translate(L"Играть"));
    referenceButton_.setText(Localizer::translate(L"Справка"));
    recordsButton_.setText(Localizer::translate(L"Рекорды"));
    exitButton_.setText(Localizer::translate(L"Выход"));

    startButton_.setText(Localizer::translate(L"Старт"));
    autoButton_.setText(Localizer::translate(L"Авто"));
    quitButton_.setText(Localizer::translate(L"Выход"));

    for(int i = 10, symbol = 1072; i < 20; i++, symbol++)
    {
        if (symbol == 1081) symbol++;
        std::wstring letter;
        letter.push_back(static_cast<wchar_t>(symbol));

        coordinates_[i].setString(Localizer::translate(letter));
    }

    window_->setTitle(Localizer::translate(L"Морской бой"));
}

void Interface::initTextures()
{
    img_.emplace("startWindow", Vector2f(gl::Width, gl::Height)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/StartWindowBackground"));

    img_.emplace("gameWindow", Vector2f(gl::screenWidth, gl::screenHeight)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/GameWindowBackground"));

    img_.emplace("referenceWindow", Vector2f(IN::refWidnowWidth, IN::refWindowHeight)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/ReferenceWindowBackground"));

    img_.emplace("recordsWindow", Vector2f(IN::recWindowWidth, IN::recWindowHeight)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/RecordsWindowBackground"));

    img_.emplace("dialogBackground", Vector2f(IN::infoWindowWidth, IN::infoWindowHeight)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/DialogWindowsBackground"));

    img_.emplace("fieldBackground", Vector2f(IN::fieldsBackWidth, IN::fieldsBackHeight)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/FieldBackground"));
    img_["fieldBackground"].setPosition(IN::fieldsBackXPos, IN::fieldsBackYPos);

    img_.emplace("timerBackground", Vector2f(IN::timeBackWidth, IN::timeBackHeight)).first->second.setTexture(
        TextureManager::getManager()->getTexture("textures/interface/windows/TimerBackground"));
    img_["timerBackground"].setPosition(IN::timeBackXPos, IN::timeBackYPos);
}

void Interface::createMainWindowButtons()
{
    auto texture = std::make_shared<Texture>(*TextureManager::getManager()->
        getTexture("textures/interface/buttons/main"));
    auto textureOn = std::make_shared<Texture>(*TextureManager::getManager()->
        getTexture("textures/interface/buttons/mainSelected"));

    playButton_ = Button(window_, Text(Localizer::translate(L"Играть"), gl::optimaFont, 
        IN::mainButtonsFontSize), IN::mainButtonsXPos, IN::playButtonYPos, IN::mainButtonsWidth, 
        IN::mainButtonsHeight, texture, textureOn, IN::textColor, IN::textColorOn);

    referenceButton_ = Button(window_, Text(Localizer::translate(L"Справка"), gl::optimaFont, 
        IN::mainButtonsFontSize), IN::mainButtonsXPos, IN::referenceButtonYPos, IN::mainButtonsWidth, 
        IN::mainButtonsHeight, texture, textureOn, IN::textColor, IN::textColorOn);

    recordsButton_ = Button(window_, Text(Localizer::translate(L"Рекорды"), gl::optimaFont, 
        IN::mainButtonsFontSize), IN::mainButtonsXPos, IN::recordsButtonYPos, IN::mainButtonsWidth, 
        IN::mainButtonsHeight, texture, textureOn, IN::textColor, IN::textColorOn);

    exitButton_ = Button(window_, Text(Localizer::translate(L"Выход"), gl::optimaFont, 
        IN::mainButtonsFontSize), IN::mainButtonsXPos, IN::exitButtonYPos, IN::mainButtonsWidth, 
        IN::mainButtonsHeight, texture, textureOn, IN::textColor, IN::textColorOn);
}

void Interface::createGameWindowButtons()
{
    auto texture = std::make_shared<Texture>(*TextureManager::getManager()->
        getTexture("textures/interface/buttons/main"));
    auto textureOn = std::make_shared<Texture>(*TextureManager::getManager()->
        getTexture("textures/interface/buttons/mainSelected"));

    startButton_ = Button(window_, Text(Localizer::translate(L"Старт"), gl::optimaFont, 
        IN::gameWindowButtonsTextFontSize), IN::startButtonXPos, IN::gameWindowButtonsYPos, 
        IN::gameWindowButtonsWidth, IN::gameWindowButtonsHeight, texture, textureOn, 
        IN::textColor, IN::textColorOn);

    autoButton_ = Button(window_, Text(Localizer::translate(L"Авто"), gl::optimaFont, 
        IN::gameWindowButtonsTextFontSize), IN::autoButtonXPos, IN::gameWindowButtonsYPos, 
        IN::gameWindowButtonsWidth, IN::gameWindowButtonsHeight, texture, textureOn, 
        IN::textColor, IN::textColorOn);

    quitButton_ = Button(window_, Text(Localizer::translate(L"Выход"), gl::optimaFont, 
        IN::gameWindowButtonsTextFontSize), IN::quitButtonXPos, IN::gameWindowButtonsYPos, 
        IN::gameWindowButtonsWidth, IN::gameWindowButtonsHeight, texture, textureOn, 
        IN::textColor, IN::textColorOn);
}

void Interface::initCoordinatesText()
{
    for(int i = 0, symbol = 1072; i < 10; i++, symbol++)
    {
        coordinates_[i].setString(std::to_string(i + 1));

        if (symbol == 1081) symbol++;
        std::wstring letter;
        letter.push_back(static_cast<wchar_t>(symbol));

        coordinates_[i + 10].setString(Localizer::translate(letter));
    }

    for(auto& text : coordinates_)
    {
        text.setFont(gl::optimaFont);
        text.setCharacterSize(static_cast<double>(gl::fieldSize) / 1.5);
        text.setFillColor(IN::textColor);
    }
}

void Interface::createShips()
{
    for(int i = 3; i >= 0; i--)
    {
        for(size_t j = 0; j < i + 1; j++) 
        {
            ships_.emplace_back(Ship(window_, 4 - i, IN::shipXPos, 
                IN::shipYPos + i * gl::fieldSize * 1.1));
        }
    }
}

void Interface::initGlobalTimeText()
{
    timeText_ = Text("00:00:00", gl::optimaFont, IN::timeTextFontSize);
    timeText_.setFillColor(IN::textColor);
    timeText_.setPosition(IN::timeTextXPos, IN::timeTextYPos);
}

void Interface::createFields()
{
    for(size_t i = 0; i < 10; i++) 
    {
        for(size_t j = 0; j < 10; j++)
        {
            leftField_.emplace_back(Field(window_, gl::xCoord + i * gl::fieldSize, 
                gl::yCoord + j * gl::fieldSize));
            rightField_.emplace_back(Field(window_, gl::xCoord + (i + 12) * gl::fieldSize, 
                gl::yCoord + j * gl::fieldSize));
        }
    }
}

Interface::Interface(std::shared_ptr<RenderWindow> window)
    : window_(window), file_(IN::maxRecordsNum),
    gameplay_(window_, &leftField_, &rightField_),
    dropDownList_(window)
{
    createShips();
    createFields();
    initTextures();
    initGlobalTimeText();
    initCoordinatesText();
    createMainWindowButtons();
    createGameWindowButtons();
}

void Interface::mainMenu()
{
    Event event;   

    while(window_->isOpen())
    {
        while(window_->pollEvent(event))
        {
            switch(event.type) {}
        }

        window_->clear();

        window_->draw(img_["startWindow"]);

        dropDownList_.drawList();

        if (playButton_.isPressed()) gameWindow();
        else if (referenceButton_.isPressed()) showReference();
        else if (recordsButton_.isPressed()) showRecords();                
        else if (exitButton_.isPressed())
        {
            if (showWarning({ 
                Localizer::translate(L"Вы уверены, что"), 
                Localizer::translate(L"хотите выйти?") }))
                window_->close(); 
        }

        std::string locale = dropDownList_.isPressed();
        if (locale.size())
        {
            Localizer::initLocalizer(locale);
            recreateTexts();
        }

        window_->display();
    }
}

void Interface::gameWindow()
{
    timeText_.setString("00:00:00");

    Thread timeThread([this]()
    {
        std::stringstream time;
        while(true)
        {
            auto [seconds, minutes, hours] = this->timer_.ticktock();

            if (hours < 10) time << 0;
            time << hours << ":";
            if (minutes < 10) time << 0;
            time << minutes << ":";
            if (seconds < 10) time << 0;
            time << seconds;

            this->timeText_.setString(time.str());
            time.str(""); 

            sleep(milliseconds(1000));
        }
    });

    int movement = -1;
    int dx, dy;
    bool checkGameStarted = false, checkPause = false;


    window_->create(VideoMode(gl::screenWidth, gl::screenHeight), 
        Localizer::translate(L"Морской бой"), Style::Fullscreen);

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
                        for(size_t i = 0; i < ships_.size(); i++)
                        {
                            if (ships_[i].onShip(x, y)) // check if the cursor is above teh ship
                            {
                                movement = i;
                                Vector2f coord = ships_[i].getPosition();
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
                        ships_[movement].setPosition(x - dx, y - dy); // if the cursore is above the ship, move it
                        ships_[movement].setFieldColor(leftField_);
                    }
                    break;
                case Event::MouseButtonReleased:
                    if (event.mouseButton.button == Mouse::Left 
                        && movement != -1 && !checkGameStarted) 
                    {
                        ships_[movement].placeShip(leftField_); // free ship
                        movement = -1;
                        dx = dy = 0;
                    }
                    else if (event.mouseButton.button == Mouse::Right 
                        && Mouse::isButtonPressed(Mouse::Left) && !checkGameStarted)
                    {
                        int x = Mouse::getPosition(*window_).x;
                        int y = Mouse::getPosition(*window_).y;
                        for(size_t i = 0; i < ships_.size(); i++)
                        {
                            if (ships_[i].onShip(x, y) && ships_[i].isPlaced()) // rotate ship relatively to cursor
                            {
                                ships_[i].rotateShip(x, y);
                                ships_[i].setFieldColor(leftField_);
                                Vector2f coord = ships_[i].getPosition();
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

        quitButton_.drawButton();
        startButton_.drawButton();
        autoButton_.drawButton();

        if (checkGameStarted && !checkPause)
        {
            if (gameplay_.startTheGame()) sleep(milliseconds(500));
        }

        for(int i = 0; i < leftField_.size(); i++) 
        {
            leftField_[i].drawField();
            rightField_[i].drawField();
        }

        drawCoordinates(gl::xCoord, gl::yCoord, gl::fieldSize);

        for(size_t i = 0; i < ships_.size(); i++) ships_[i].drawShip();

        window_->draw(timeText_);

        if (checkGameStarted) gameplay_.drawPositions();

        if (checkGameStarted && !checkPause)
        {
            if (gameplay_.checkEndGame())
            {
                std::wstring result = gameplay_.getResult();
                if (result == Localizer::translate(L"Победа!"))
                {
                    file_.writeRecordsToFile({ timeText_.getString() });
                }
                showMessage({ result });
                break;
            }
        }

        if (startButton_.isPressed())
        {
            bool flag = true;

            if (!checkGameStarted)
            {
                // check if all the sips were placed 
                for(const auto& ship : ships_) 
                {
                    if (ship.isPlaced())
                    {
                        flag = false;
                        showMessage({ 
                            Localizer::translate(L"Корабли неверно"), 
                            Localizer::translate(L"расставлены!") });
                        break;
                    }
                }
            }

            // if all the ships were placed, start the game
            if (flag)
            {
                if (!checkGameStarted) 
                {
                    timeThread.launch();
                    checkGameStarted = true;
                    gameplay_.placeShips(&rightField_, &ships_);
                    startButton_.setText(Localizer::translate(L"Пауза"));
                    startButton_.setTextFontSize(
                        IN::gameWindowButtonsTextFontSize);
                }
                else
                {
                    if(!checkPause)
                    {
                        timeThread.terminate();
                        checkPause = true;
                        startButton_.setText(Localizer::translate(L"Продолжить"));
                        startButton_.setTextFontSize(
                            IN::startButtonContinueTextFontSize);
                    }
                    else
                    {
                        timeThread.launch();
                        checkPause = false;
                        startButton_.setText(Localizer::translate(L"Пауза"));
                        startButton_.setTextFontSize(
                            IN::gameWindowButtonsTextFontSize);
                    }
                }      
            }
            else // else reset ships positions
            {
                for(auto& ship : ships_) ship.resetPosition(leftField_);
            }
        }
        else if (autoButton_.isPressed() && !checkGameStarted)
        {
            for(auto& ship : ships_) ship.resetPosition(leftField_);
            for(auto& field : leftField_) field.resetData();

            gameplay_.placeShips(&leftField_, &ships_);           
        }
        else if (quitButton_.isPressed())
        {
            if (showWarning({ 
                Localizer::translate(L"Вы уверены, что"), 
                Localizer::translate(L"хотите выйти?") })) break;
        }
        
        window_->display();
    }

    timeThread.terminate();
    reinitGameParams();

    window_->create(VideoMode(gl::Width, gl::Height), 
        Localizer::translate(L"Морской бой"), Style::Close);
    window_->setPosition(Vector2i((gl::screenWidth - gl::Width) 
        / 2, (gl::screenHeight - gl::Height) / 2)); 
}

void Interface::showReference() const
{
    RenderWindow refWindow(VideoMode(IN::refWidnowWidth, IN::refWindowHeight), 
        Localizer::translate(L"Справка"), Style::Close);

    int windowXPos = window_->getPosition().x + 
        (window_->getSize().x - IN::refWidnowWidth) / 2;
    int windowYPos = window_->getPosition().y + 
        (window_->getSize().y - IN::refWindowHeight) / 2;

    refWindow.setPosition(Vector2i(windowXPos, windowYPos));

    const std::vector<std::wstring> rulesStrings{
        Localizer::translate(L"В начале игры вам необходимо расставить свои"),
        Localizer::translate(L"корабли на левом поле:"),
        Localizer::translate(L"1 - четырехпалубный"),
        Localizer::translate(L"2 - трехпалубных"),
        Localizer::translate(L"3 - двухпалубных"),
        Localizer::translate(L"4 - однопалубных"),
        Localizer::translate(L"Расстановка кораблей производится путем пере-"),
        Localizer::translate(L"таскивания их на игровое поле. Корабли не мо-"),
        Localizer::translate(L"гут касаться сторонами или углами. Первым хо-"),
        Localizer::translate(L"дит игрок."),
        Localizer::translate(L"В случае попадания игроком по кораблю против-"),
        Localizer::translate(L"ника, этот игрок продолжает свой ход. В ином"),
        Localizer::translate(L"случае, ход переходит другому игроку. Игра за-"),
        Localizer::translate(L"канчивается, когда убиты все корабли одного из"),
        Localizer::translate(L"игроков (корабль считается убитым тогда, когда"),
        Localizer::translate(L"у него подбиты все палубы).")
    };

    std::vector<Text> rulesText;
    for(int i = 0; i < rulesStrings.size(); i++)
    {
        rulesText.emplace_back(Text(rulesStrings[i], gl::optimaFont, IN::rulesTextFontSize));
        rulesText[i].setFillColor(IN::textColor);
        rulesText[i].setPosition(IN::rulesTextXPos, IN::rulesTextYPos + 
            i * IN::rulesTextInterval);
    }

    Event event;

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

        refWindow.clear(Color::White);

        refWindow.draw(img_["referenceWindow"]);

        for(const auto& str : rulesText) refWindow.draw(str);

        refWindow.display();
    }
}

void Interface::showRecords() const
{
    int windowXPos = window_->getPosition().x + 
        (window_->getSize().x - IN::recWindowWidth) / 2;
    int windowYPos = window_->getPosition().y + 
        (window_->getSize().y - IN::recWindowHeight) / 2;

    auto records = file_.readRecordsFromFile();

    std::vector<Text> recordsTexts(records.size());
    for(int i = 0; i < records.size(); i++)
    {
        recordsTexts[i] = Text(std::to_string(i + 1) + ") " + records[i], 
            gl::optimaFont, IN::recordsTextFontSize);
        recordsTexts[i].setFillColor(IN::textColor);
        recordsTexts[i].setPosition(IN::recordsXPos, 
            gl::screenHeight / 6 + i * 1.2 * recordsTexts[i].getGlobalBounds().height);
    }

    RenderWindow recordsWindow(VideoMode(IN::recWindowWidth, IN::recWindowHeight), 
        Localizer::translate(L"Рекорды"), Style::Close);
    recordsWindow.setPosition(Vector2i(windowXPos, windowYPos));

    Text titleText(Localizer::translate(L"Рекорды"), 
        gl::optimaFont, IN::recWindowTitleTextFontSize);
    titleText.setFillColor(IN::textColor);
    titleText.setPosition((IN::recWindowWidth - 
        titleText.getGlobalBounds().width) / 2, IN::recWindowTitleTextYPos);

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
