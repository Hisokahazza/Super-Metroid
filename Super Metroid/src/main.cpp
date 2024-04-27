#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "Game.h"
#include "Camera.h"


int main()
{
	// Create a window and clock
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Metroid");
	sf::Clock deltaClock;

	Renderer renderer(window);

	Game game;

	// Limit fps
	window.setFramerateLimit(60);

	// Run begin function
	game.Begin(window);

	while (window.isOpen())
	{
		// Determine deltatime each frame
		float deltaTime = deltaClock.restart().asSeconds();

		// Poll events
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Set camera view
		window.setView(camera.getView(window.getSize()));

		// Call update
		game.update(deltaTime);

		// Clears screen before next render
		window.clear(sf::Color(20, 20, 20));

		game.draw(renderer);

		// Set camera view (HUD)
		window.setView(camera.getHUDView());

		game.drawUI(renderer);

		game.drawMenu(renderer);

		window.display();
	}
}