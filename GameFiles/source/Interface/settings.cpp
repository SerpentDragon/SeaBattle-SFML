#include "settings.hpp"

// global namespace
namespace gl
{
    const int screenWidth = sf::VideoMode::getDesktopMode().width;
    const int screenHeight = sf::VideoMode::getDesktopMode().height;
    const int Width = 0.625 * screenWidth; // 1200
    const int Height = 0.741 * screenHeight; // 800
    const int startWindowXPos = (gl::screenWidth - gl::Width) / 2;
    const int startWindowYPos = (gl::screenHeight - gl::Height) / 2;
    const int xCoord = 0.23 * screenWidth;
    const int yCoord = 0.206 * screenHeight;
    const int fieldSize = 0.034 * screenWidth; // 65

    sf::Font arialFont;
    sf::Font trebuchetFont;
    sf::Font optimaFont;

    void initFonts()
    {
        arialFont.loadFromFile(std::string(INSTALL_DIR) + "app_data/fonts/arial.ttf");
        trebuchetFont.loadFromFile(std::string(INSTALL_DIR) + "app_data/fonts/trebuchetms.ttf");
        optimaFont.loadFromFile(std::string(INSTALL_DIR) + "app_data/fonts/optima.ttf");
    }

    const sf::Color buttonColor(180, 180, 180);
    const sf::Color buttonColorOn(0, 191, 255);
}

// Interface namespace
namespace IN
{
    // Windows params
    const int refWidnowWidth = 0.5 * gl::Width;
    const int refWindowHeight = 0.6 * gl::Height;
    const int recWindowWidth = gl::screenWidth / 5;
    const int recWindowHeight = gl::screenHeight / 2;
    const int infoWindowWidth = gl::fieldSize * 7;
    const int infoWindowHeight = gl::fieldSize * 4;
    const int infoWindowTextFontSize = gl::Height / 20;

    // Buttons params
    const int mainButtonsWidth = gl::Width / 4;
    const int mainButtonsHeight = gl::fieldSize;
    const int mainButtonsFontSize = 0.053 * gl::Height;
    const int mainButtonsXPos = 0.72 * gl::Width;

    const int gameWindowButtonsWidth = 23 * gl::screenWidth / 192;
    const int gameWindowButtonsHeight = 85 * gl::screenHeight / 1080;
    const int startButtonXPos = gl::screenWidth / 24;
    const int autoButtonXPos = 355 * gl::screenWidth / 1920;
    const int quitButtonXPos = 5 * gl::screenWidth / 6;
    const int gameWindowButtonsYPos = 0.88 * gl::screenHeight;
    const int gameWindowButtonsTextFontSize = 0.025 * gl::screenWidth;
    const int startButtonContinueTextFontSize = 0.016 * gl::screenWidth;

    const int dialogButtonsWidth = gl::fieldSize * 2;
    const int dialogButtonsHeight = gl::fieldSize;

    const int playButtonYPos = 0.3375 * gl::Height;
    const int referenceButtonYPos = playButtonYPos + 0.11 * gl::Height;
    const int recordsButtonYPos = referenceButtonYPos + 0.11 * gl::Height;
    const int exitButtonYPos = recordsButtonYPos + 0.11 * gl::Height;

    const int infoWindowButtonWidth = gl::fieldSize * 2;
    const int infoWindowButtonHeight = gl::fieldSize;
    const int infoWindowYesButtonXPos = gl::fieldSize;
    const int infoWindowNoButtonXPos = gl::fieldSize * 4;
    const int infoWindowButtonYPos = gl::fieldSize * 2.5;
    const int infoWindowButtonFontSize = gl::Width / 40;

    // Ships params
    const int shipXPos = gl::Width / 24;
    const int shipYPos = 350 * gl::screenHeight / 1080;

    // Records params
    const int maxRecordsNum = 5;
    const int recordsXPos = 70 * IN::recWindowWidth / 384;
    const int recordsTextFontSize = gl::Width / 24;
    const int recWindowTitleTextYPos = IN::recWindowHeight / 20;
    const int recWindowTitleTextFontSize = IN::recWindowHeight / 10;

    // Time text params
    const int timeTextFontSize = 0.0265 * gl::screenWidth;
    const int timeTextXPos = 3 * gl::screenWidth / 8;
    const int timeTextYPos = 964 * gl::screenHeight / 1080;

    // Rules text params
    const int rulesTextFontSize = 0.0201 * gl::Width;
    const int rulesTextXPos = gl::Width / 40;
    const int rulesTextYPos = 3 * gl::Height / 100;
    const int rulesTextInterval = 0.0675 * gl::Width / 3;

    // Substrates params
    const int fieldsBackWidth = 24 * gl::fieldSize;
    const int fieldsBackHeight = 12 * gl::fieldSize;
    const int fieldsBackXPos = 70 * gl::fieldSize / 13;
    const int fieldsBackYPos = 2 * gl::fieldSize;

    const int timeBackWidth = gl::fieldSize * 5;
    const int timeBackHeight = gl::fieldSize * 1.3;
    const int timeBackXPos = 0.33 * gl::screenWidth;
    const int timeBackYPos = 0.88 * gl::screenHeight;

    // Colors
    const sf::Color textColor(85, 4, 29);
    const sf::Color textColorOn(0, 36, 91);
}

// Field namespace
namespace FI
{
    const int fieldOutlineThickness = gl::screenHeight / 500;
}

// DropDown List namespace
namespace DR_LS
{
    // DropDown List params
    const int dropDownListSize = 50 * gl::fieldSize / 65;
    const int droDownListXPos = gl::Width - DR_LS::dropDownListSize * 1.5;
    const int dropDownListYPos = DR_LS::dropDownListSize / 2;

    // DropDown List colors
    const sf::Color DARK_BROWN(86, 3, 29);
    const sf::Color BROWN(191,110,63);
}
