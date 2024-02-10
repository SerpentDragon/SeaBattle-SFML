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

    auto window = std::make_shared<RenderWindow>(VideoMode(gl::Width, gl::Height), 
        L"Морской бой", Style::Close);
    window->setPosition(Vector2i((gl::screenWidth - gl::Width) / 2, (gl::screenHeight - gl::Height) / 2));

    Interface interface(window);

    interface.mainMenu();

    return 0;
}
