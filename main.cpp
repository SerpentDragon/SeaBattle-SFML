#include <SFML/Graphics.hpp>
#include <iostream>
#include "Interface.h"
#include "settings.h"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(Width, Height), L"Морской бой", Style::Close);
    window.setPosition(Vector2i((screenWidth - Width) / 2, (screenHeight - Height) / 2));
    
    Interface interface(&window);

    interface.mainMenu();

    return 0;
}