#pragma once

#include <cmath>
#include <queue>

#include <iostream>

#include <box2d/b2_fixture.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>

#include "Renderer.h"
#include "Animation.h"
#include "Physics.h"

#include "MenuManager.h"

extern MenuManager menuManager;

// bundle attributes in a struct
struct BossAttributes
{
	unsigned int health;
	float defenceMultiplier;
};

enum BossName
{
	SPORESPAWN,
	GOLDTORIZO
};

enum BossAnimationState
{
	// SporeSpawn animation states
	COREOPENING,
	COREOPENED,
	CORECLOSING,
	CORECLOSED,
	COREFLASHING,

	// Gold Torizo animation states
	GOLDTORIZOBLINK,
	GOLDTORIZOSTAND,
	GOLDTORIZOTRANSITION,
	GOLDTORIZOWALKLEFT,
	GOLDTORIZOWALKRIGHT,
	BOMBSPEWLEFT
	
};

class Boss : public Collisionlistener
{
private:
protected:
	b2Body* body;
	BossAttributes attributes;
	int playerHealthOffset = 0;
	FixtureData* projectileDestroyed;

	std::unordered_map<BossAnimationState, SheetlessAnimation*> m_SheetlessAnimations;
	std::vector<BossAnimationState> m_ActiveStates = {};
	BossAnimationState m_CurrentAnimationState;

	FixtureData fixtureData{};
public:
	b2Fixture* playerHitbox;
	bool isPlayerInvulnerable = false;
	
	bool m_IsSamusHit = false;
	sf::Vector2f samusPosition;
	sf::Vector2f position;

	virtual void createFixture() = 0;
	virtual void resetFixture() = 0;

	virtual void begin() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;

	// Getters and setters
	int const getPlayerHealthOffset() { return playerHealthOffset; }
	void setPlayerHealthOffset(int newOffset) { playerHealthOffset = newOffset; }

	bool const getIsSamusHit() { return m_IsSamusHit; }
	void const setIsSamusHit(bool isSamusHit) { m_IsSamusHit = isSamusHit; }

	FixtureData* const getProjectileDestroyed() { return projectileDestroyed; }

	void setPlayerHitbox(b2Fixture* hitbox) { playerHitbox = hitbox; }

	void setPlayerinvulnerabillity(bool isInvulnerable) { isPlayerInvulnerable = isInvulnerable; }

	void setSamusPosition(sf::Vector2f position) { samusPosition = position; }
};

class BossComponent : public Collisionlistener
{
private:
protected:
	b2Body* body;

	sf::Vector2f position;
	virtual void createFixture() = 0;

	FixtureData* projectileDestroyed;
	b2Fixture* m_PlayerHitbox;
	bool m_IsPlayerInvulnerable;

public:
	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;

	// Getters and setters
	void setPlayerHitbox(b2Fixture* hitbox) { m_PlayerHitbox = hitbox; }
	void setPlayerinvulnerabillity(bool isInvulnerable) { m_IsPlayerInvulnerable = isInvulnerable; }
	FixtureData* const getProjectileDestroyed() { return projectileDestroyed; }
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
	
	void createFixture() override;

	std::vector<sf::Texture> m_SporeFrames;
	SheetlessAnimation* m_SporeAnimation;
public:
	bool destroyed = false;
	bool collided = false;

	~Spore();

	void begin(unsigned int positionIndex);
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	int const getPlayerHealthOffset() { return m_PlayerHealthOffset; };

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

class SporeSpawn : public Boss
{
private:
	void createFixture() override;

	void closeCore(float deltaTime);
	void openCore(float deltaTime);

	void createActiveAnimations();

	sf::Vector2f determineSegmentPos(float positionOnLine);

	sf::Vector2f m_TetherPoint = {7.5f, 5.0f};

	float m_SporeSwitchTime = 2.0f;
	float m_SporeTotalTime = 2.0f;

	float m_CoreTotalTime = 0.0f;
	float m_CoreOpenSwitchTime = 10.0f;
	float m_CoreClosedSwitchTime = 5.0f;

	int m_BossSpeed = 3;
	bool m_BossChangedDirection = false;

	Spore* m_Spore;
	std::vector<Spore*> m_Spores;
	unsigned int m_SpawnPosIndexSpore = 0;

	b2Fixture* m_CoreOpenTop;
	b2Fixture* m_CoreOpenBottom;
	b2Fixture* m_CoreClosed;
	b2Fixture* m_CoreFixture;

	bool m_IsCoreOpen = false;
	bool m_IsCoreClosing = false;
	bool m_IsCoreHit = false;
	bool m_IsHittable = false;


	bool m_BossComplete = false;
public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	void resetFixture() override;

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

class GoldTorizo : public Boss
{
private:
	void createFixture() override;
	void createActiveAnimations();

	Direction m_Orientation;
	std::queue<BossAnimationState> m_BossActions;

	bool m_IntroOver = false;

public:
	void begin() override;
	void update(float deltaTime);
	void draw(Renderer& renderer);

	void resetFixture();

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

