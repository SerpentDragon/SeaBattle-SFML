#pragma once

#include "absolute_path.h"
#include <SFML/Graphics.hpp>

// global namespace
namespace gl
{
    extern const int screenWidth;
    extern const int screenHeight;
    extern const int Width;
    extern const int Height;
    extern const int startWindowXPos;
    extern const int startWindowYPos;
    extern const int xCoord;
    extern const int yCoord;
    extern const int fieldSize;

    extern sf::Font arialFont;
    extern sf::Font trebuchetFont;
    extern sf::Font optimaFont;

    void initFonts();

    extern const sf::Color buttonColor;
    extern const sf::Color buttonColorOn;
}

// Interface namespace
namespace IN
{
    // Windows params
    extern const int refWidnowWidth;
    extern const int refWindowHeight;
    extern const int recWindowWidth;
    extern const int recWindowHeight;
    extern const int infoWindowWidth;
    extern const int infoWindowHeight;
    extern const int infoWindowTextFontSize;

    // Buttons params
    extern const int mainButtonsWidth;
    extern const int mainButtonsHeight;
    extern const int mainButtonsFontSize;
    extern const int mainButtonsXPos;

    extern const int gameWindowButtonsWidth;
    extern const int gameWindowButtonsHeight;
    extern const int startButtonXPos;
    extern const int autoButtonXPos;
    extern const int quitButtonXPos;
    extern const int gameWindowButtonsYPos;
    extern const int gameWindowButtonsTextFontSize;
    extern const int startButtonContinueTextFontSize;

    extern const int dialogButtonsWidth;
    extern const int dialogButtonsHeight;

    extern const int playButtonYPos;
    extern const int referenceButtonYPos;
    extern const int recordsButtonYPos;
    extern const int exitButtonYPos;

    extern const int infoWindowButtonWidth;
    extern const int infoWindowButtonHeight;
    extern const int infoWindowYesButtonXPos;
    extern const int infoWindowNoButtonXPos;
    extern const int infoWindowButtonYPos;
    extern const int infoWindowButtonFontSize;

    // Ships params
    extern const int shipXPos;
    extern const int shipYPos;

    // Records params
    extern const int maxRecordsNum;
    extern const int recordsXPos;
    extern const int recordsTextFontSize;
    extern const int recWindowTitleTextYPos;
    extern const int recWindowTitleTextFontSize;

    // Time text params
    extern const int timeTextFontSize;
    extern const int timeTextXPos;
    extern const int timeTextYPos;

    // Rules text params
    extern const int rulesTextFontSize;
    extern const int rulesTextXPos;
    extern const int rulesTextYPos;
    extern const int rulesTextInterval;


    // Substrates params
    extern const int fieldsBackWidth;
    extern const int fieldsBackHeight;
    extern const int fieldsBackXPos;
    extern const int fieldsBackYPos;

    extern const int timeBackWidth;
    extern const int timeBackHeight;
    extern const int timeBackXPos;
    extern const int timeBackYPos;

    // Colors
    extern const sf::Color textColor;
    extern const sf::Color textColorOn;
}

// Field namespace
namespace FI
{
    extern const int fieldOutlineThickness;
}

// DropDown List namespace
namespace DR_LS
{
    // DropDown List params
    extern const int dropDownListSize;
    extern const int droDownListXPos;
    extern const int dropDownListYPos;

    // DropDown List colors
    extern const sf::Color DARK_BROWN;
    extern const sf::Color BROWN;
}
