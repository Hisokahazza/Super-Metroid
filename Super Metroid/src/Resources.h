#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

struct Resources
{
	static std::unordered_map <std::string, sf::Texture> textures;
};