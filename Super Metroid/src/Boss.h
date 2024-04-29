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
	FixtureData* projectileDestroyed;

	std::unordered_map<BossAnimationState, Animation*> m_SheetlessAnimations;
	std::vector<BossAnimationState> m_ActiveStates = {};
	BossAnimationState m_CurrentAnimationState;
public:
	b2Fixture* playerHitbox;
	bool isPlayerInvulnerable = false;
	bool m_IsSamusHit = false;

	sf::Vector2f position;

	virtual void createFixture() = 0;

	virtual void begin() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;

	int const getPlayerHealthOffset() { return playerHealthOffset; };
	void setPlayerHealthOffset(int newOffset) { playerHealthOffset = newOffset; };

	bool const getIsSamusHit() { return m_IsSamusHit; };
	void const setIsSamusHit(bool isSamusHit) { m_IsSamusHit = isSamusHit; };

	FixtureData* const getProjectileDestroyed() { return projectileDestroyed; }

	void setPlayerHitbox(b2Fixture* hitbox) { playerHitbox = hitbox; };

	void setPlayerinvulnerabillity(bool isInvulnerable) { isPlayerInvulnerable = isInvulnerable; };
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

	void setPlayerHitbox(b2Fixture* hitbox) { m_PlayerHitbox = hitbox; };
	void setPlayerinvulnerabillity(bool isInvulnerable) { m_IsPlayerInvulnerable = isInvulnerable; };
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
	bool switchScreens = false;

	void begin();
	void update(float deltaTime);
	void draw(Renderer& renderer);

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

