#include <SFML/Graphics.hpp>
#include <ctime>
#include "Interface.h"
#include "settings.h"
#include "Field.h"

using namespace sf;

int screenWidth = VideoMode::getDesktopMode().width;
int screenHeight = VideoMode::getDesktopMode().height;
int Width = 0.624 * screenWidth;
int Height = 0.745 * screenHeight;
int fieldSize = 0.034 * screenWidth;
int xCoord = 0.23 * screenWidth;
int yCoord = 0.206 * screenHeight;

Font arialFont;
Font trebuchetFont;
Font optimaFont;

Color buttonColor(180, 180, 180);
Color buttonColorOn(0, 191, 255);

Texture playerHitTexture, playerMissedTexture, computerHitTexture, computerMissedTexture;

void initFonts()
{
    arialFont.loadFromFile("fonts/arial.ttf");
    trebuchetFont.loadFromFile("fonts/trebuchetms.ttf");
    optimaFont.loadFromFile("fonts/optima.ttf");
}

void initTextures()
{
    playerHitTexture = *TextureManager::getManager()->
        getTexture("textures/marks/playerHit");
    playerMissedTexture = *TextureManager::getManager()->
        getTexture("textures/marks/playerMissed");
    computerHitTexture = *TextureManager::getManager()->
        getTexture("textures/marks/computerHit");
    computerMissedTexture = *TextureManager::getManager()->
        getTexture("textures/marks/computerMissed");
}

int main()
{
    TextureManager::getManager();

    initFonts();
    initTextures();

    srand(time(0));

    RenderWindow window(VideoMode(Width, Height), L"Морской бой", Style::Close);
    window.setPosition(Vector2i((screenWidth - Width) / 2, (screenHeight - Height) / 2));

    Interface interface(&window);

    interface.mainMenu();

    return 0;
}
