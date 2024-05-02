#pragma once

#include "Renderer.h"
#include <vector>
#include <filesystem>

class Animation
{
private:
public:
	bool playing = true;

	virtual sf::Texture getCurrentFrame() = 0;
	virtual void begin() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void reset() = 0;

	inline bool const checkPlaying() { return playing; }
};

class SheetAnimation : public Animation
{
private:
	sf::IntRect m_InitialRect;
	sf::IntRect m_CurrentRect;
	sf::Vector2u m_SheetDimensions;
	sf::Vector2u m_RowsColumns;
	sf::Texture m_SpriteSheet;
	std::string m_SheetTag;

	bool m_IsStatic;
	unsigned int m_KeyFrames;

	sf::Texture m_CurrentFrame;

	float m_TotalTime = 0.0f;
	float m_SwitchTime;

	unsigned int m_FrameCount = 0;
	bool m_AnimDirection = true;
	int m_TimesPlayed;
	bool m_Loop;
	
public:
	SheetAnimation(const sf::Texture& spriteSheet = sf::Texture(), const std::string& sheetTag = "", const sf::Vector2u& rowsColumns = {}, unsigned int keyFrames = 0, bool isStatic = false, float switchTime = 0.1, int timesPlayed = -1, bool loop = true);

	void begin();
	void update(float deltaTime);
	void reset();

	sf::Texture getCurrentFrame();

	
};

class SheetlessAnimation : public Animation
{
private:
	std::vector<sf::Texture> m_AnimationTextures;

	float m_SwitchTime;
	float m_TotalTime = 0.0f;

	bool m_Loop;

	unsigned int m_NumTextures;
	int m_NextTextureIndex;

	sf::Texture m_CurrentTexture;

	bool m_AnimDirection = true;
	int m_TimesPlayed;
	bool m_Reverse;

	int m_FrameCount = 0;

	std::vector<sf::Vector2f> m_FrameSizes;
	sf::Vector2f m_CurrentTextureSize;
public:
	SheetlessAnimation(std::vector<sf::Texture> AnimationTextures, float switchTime = 0.0f, bool loop = false, int timesPlayed = -1, bool reverse = false, std::vector<sf::Vector2f> frameSizes = {});

	void begin();
	void update(float deltaTime);
	void reset();

	sf::Texture getCurrentFrame() { return m_CurrentTexture; };
	sf::Vector2f getCurrentFrameSize() { return m_CurrentTextureSize; };
};