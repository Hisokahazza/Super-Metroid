#pragma once

#include <cmath>

#include <iostream>

#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>


#include "Renderer.h"
#include "Animation.h"
#include "Physics.h"

struct BossAttributes
{
	unsigned int health;
	float defenceMultiplier;
};

enum BossAnimationState
{
	COREOPENING,
	COREOPENED,
	CORECLOSING,
	CORECLOSED,
	COREFLASHING
};

class Boss : public Collisionlistener
{
private:
protected:
	b2Body* body;
	BossAttributes attributes;
	int playerHealthOffset = 0;

	std::unordered_map<BossAnimationState, SheetlessAnimation*> m_SheetlessAnimations;
	std::vector<BossAnimationState> m_ActiveStates = {};
	BossAnimationState m_CurrentAnimationState;
public:
	b2Fixture* playerHitbox;

	sf::Vector2f position;

	virtual void createFixture() = 0;

	virtual void begin() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;

	int const getPlayerHealthOffset() { return playerHealthOffset; };
	void setPlayerHealthOffset(int newOffset) { playerHealthOffset = newOffset; };

	void setPlayerHitbox(b2Fixture* hitbox) { playerHitbox = hitbox; };
};

class BossComponent : public Collisionlistener
{
private:
protected:
	b2Body* body;

	sf::Vector2f position;
	virtual void createFixture() = 0;

	b2Fixture* m_PlayerHitbox;

public:
	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;

	void setPlayerHitbox(b2Fixture* hitbox) { m_PlayerHitbox = hitbox; };
};

class Spore : public BossComponent
{
private:
	int m_PlayerHealthOffset = 0;

	FixtureData m_SporeFixtureData;
	b2Fixture* m_SporeFixture;

	float m_SwitchTime = 0.01f;
	float m_TotalTime = 0.0f;

	std::vector<sf::Vector2f> m_SporeInitialPositions = {sf::Vector2f(3.0f, 2.3f), sf::Vector2f(6.0f, 2.3f) , sf::Vector2f(9.0f, 2.3f) , sf::Vector2f(12.0f, 2.3f) };
	
	void createFixture();

	std::vector<sf::Texture> m_SporeFrames;
	SheetlessAnimation* m_SporeAnimation;
public:
	
	bool destroyed = false;
	bool collided = false;

	~Spore();

	void begin(unsigned int positionIndex);
	void update(float deltaTime);
	void draw(Renderer& renderer);

	int const getPlayerHealthOffset() { return m_PlayerHealthOffset; };

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

class SporeSpawn : public Boss
{
private:
	FixtureData m_FixtureData{};
	void createFixture();
	void createActiveAnimations();

	float m_SporeSwitchTime = 2.0f;
	float m_SporeTotalTime = 2.0f;

	int m_BossSpeed = 3;
	bool m_BossChangedDirection = false;

	Spore* m_Spore;
	std::vector<Spore*> m_Spores;
	unsigned int m_SpawnPosIndexSpore = 0;

	b2Fixture* m_CoreOpenTop;
	b2Fixture* m_CoreOpenBottom;
	b2Fixture* m_CoreClosed;
	b2Fixture* m_CoreFixture;

	bool m_CoreOpen = true;
	bool m_CoreClosing = false;
	bool m_Hittable = false;

	FixtureData* m_ProjectileDestroyed;
public:
	void begin();
	void update(float deltaTime);
	void draw(Renderer& renderer);

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;

	FixtureData* const getProjectileDestroyed() { return m_ProjectileDestroyed; }
};

