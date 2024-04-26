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
		m_ViewSize = sf::Vector2f(m_ZoomLevel * scaleView.x, m_ZoomLevel / aspect * scaleView.y);
	else
		m_ViewSize = sf::Vector2f(m_ZoomLevel * aspect * scaleView.x, m_ZoomLevel * scaleView.y);

// return a view type object
	return sf::View(position, m_ViewSize);
}

sf::View Camera::getHUDView()
{
	float aspect = m_ViewSize.x / m_ViewSize.y;
	m_ViewSize = sf::Vector2f(100.0f * scaleView.x, (100.0f / aspect) * scaleView.y);
	return sf::View(sf::Vector2f(), m_ViewSize);
}
