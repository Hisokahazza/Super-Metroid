#include "Renderer.h"

// Load Textures from a folder and add them to resources unordered map
void Renderer::loadTex()
{
	for (auto& file : std::filesystem::directory_iterator("./res/textures/"))
	{
		if (file.is_regular_file() && (file.path().extension() == ".png" ||
			file.path().extension() == ".jpg" || file.path().extension() == ".gif" || file.path().extension() == ".jpeg"))
		{
			Resources::textures[file.path().filename().string()].loadFromFile(
				file.path().string());
		}
	}
}

Renderer::Renderer(sf::RenderTarget& target) : target(target)
{
	loadTex();
}

Renderer::~Renderer()
{
}

void Renderer::draw(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& size, float angle, sf::IntRect textureRect)
{
	m_Sprite.setTexture(texture, true);
	m_Sprite.setOrigin((sf::Vector2f)texture.getSize() / 2.0f);
	m_Sprite.setPosition(position);
	m_Sprite.setScale(sf::Vector2f(size.x / texture.getSize().x, size.y / texture.getSize().y));
	m_Sprite.setRotation(angle);

	if (textureRect != sf::IntRect())
	{
		m_Sprite.setTextureRect(textureRect);
	}

	target.draw(m_Sprite);
}

void Renderer::drawRectangleShape(sf::RectangleShape& rectangle, const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& fillColour)
{
	rectangle.setPosition(position);
	rectangle.setSize(size);
	rectangle.setFillColor(fillColour);

	target.draw(rectangle);
}

void Renderer::drawText(sf::Text& text, const sf::Vector2f& position, const sf::Vector2f& size, std::string& textContents)
{
	sf::Font superMetroidFont;
	superMetroidFont.loadFromFile("res/Fonts/super-metroid-small-alt-snes.ttf");

	text.setFont(superMetroidFont);
	text.setPosition(position);
	text.setScale(size);
	text.setString(textContents);

	target.draw(text);
}
