#include <SFML/Graphics.hpp>
#include "Interface.h"
#include "settings.h"

using namespace sf;

int screenWidth = VideoMode::getDesktopMode().width;
int screenHeight = VideoMode::getDesktopMode().height;
int Width = 0.624 * screenWidth;
int Height = 0.745 * screenHeight;

Font arialFont;
Font trebuchetFont;

void initFonts()
{
    arialFont.loadFromFile("fonts/arial.ttf");
    trebuchetFont.loadFromFile("fonts/trebuchetms.ttf");
}

int main()
{
    initFonts();
    
    RenderWindow window(VideoMode(Width, Height), L"Морской бой", Style::Close);
    window.setPosition(Vector2i((screenWidth - Width) / 2, (screenHeight - Height) / 2));  
    
    Interface interface(&window);

    interface.mainMenu();

    return 0;
}
