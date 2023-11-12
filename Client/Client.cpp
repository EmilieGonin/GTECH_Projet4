#include <SFML/Graphics.hpp>
#include "Client.h"
#include "Window.h"

int main()
{
    Window window;

    //Add shape
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    window.addShape(&shape);

    //Loop
    while (window.isOpen()) window.update();

    return 0;
}