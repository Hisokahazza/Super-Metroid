#include "Camera.h"

Camera::Camera(float zoomLevel) : m_ZoomLevel(zoomLevel)
{
}

Camera::~Camera()
{
}

sf::View Camera::getView(sf::Vector2u windowSize)
{
// Ensures that when window is resized rendered image remains constant
	float aspect = (float)windowSize.x / (float)windowSize.y;
	if (aspect < 1.0f)
		m_ViewSize = sf::Vector2f(m_ZoomLevel, m_ZoomLevel / aspect);
	else
		m_ViewSize = sf::Vector2f(m_ZoomLevel * aspect, m_ZoomLevel);

// return a view type object
	return sf::View(position, m_ViewSize);
}

sf::View Camera::getHUDView()
{
	float aspect = m_ViewSize.x / m_ViewSize.y;
	m_ViewSize = sf::Vector2f(100.0f, 100.0f / aspect);
	return sf::View(sf::Vector2f(), m_ViewSize);
}
