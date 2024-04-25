#pragma once

#include <SFML/Graphics.hpp>

class Camera
{
private:
	float m_ZoomLevel;
	sf::Vector2f m_ViewSize;

public:
	Camera(float zoomLevel = 5.0f);
	~Camera();

	sf::View getView(sf::Vector2u windowSize);
	sf::View getHUDView();
	sf::Vector2f position;
};