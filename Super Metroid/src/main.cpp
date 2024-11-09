#include <SFML/Graphics.hpp>

#include "Renderer.h"
#include "Game.h"
#include "Camera.h"
#include "imgui-SFML.h"


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
	game.begin(window);

	// Init Imgui
	ImGui::SFML::Init(window);

	while (window.isOpen())
	{
		// Determine deltatime each frame
		float deltaTime = deltaClock.restart().asSeconds();

		// Poll events
		sf::Event event{};
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Set camera view
		window.setView(camera.getView(window.getSize()));

		// Update ImGui
		ImGui::SFML::Update(window, deltaClock.restart());

		// Call update
		game.update(deltaTime);

		// Clears screen before next render
		window.clear(sf::Color(0, 0, 0));

		game.draw(renderer);

		// Set camera view (HUD)
		window.setView(camera.getHUDView());

		game.drawUI(renderer);

		game.drawMenu(renderer);

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
}