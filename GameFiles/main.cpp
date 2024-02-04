#include <ctime>
#include <SFML/Graphics.hpp>
#include "source/Elements/Field.hpp"
#include "source/Interface/settings.hpp"
#include "source/Interface/Interface.hpp"

using namespace sf;

int main()
{
    TextureManager::getManager();

    gl::initFonts();

    srand(time(nullptr));

    RenderWindow window(VideoMode(gl::Width, gl::Height), L"Морской бой", Style::Close);
    window.setPosition(Vector2i(gl::startWindowXPos, gl::startWindowYPos));

    Interface interface(&window);

    interface.mainMenu();

    return 0;
}
