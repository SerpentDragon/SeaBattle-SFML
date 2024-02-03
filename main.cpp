#include <SFML/Graphics.hpp>
#include <ctime>
#include "Interface.hpp"
#include "settings.hpp"
#include "Field.hpp"

using namespace sf;

Texture playerHitTexture, playerMissedTexture, computerHitTexture, computerMissedTexture;

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

    gl::initFonts();
    initTextures();

    srand(time(nullptr));

    RenderWindow window(VideoMode(gl::Width, gl::Height), L"Морской бой", Style::Close);
    window.setPosition(Vector2i(gl::startWindowXPos, gl::startWindowYPos));

    Interface interface(&window);

    interface.mainMenu();

    return 0;
}
