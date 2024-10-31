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
	PROJECTILESPEWLEFT,
	PROJECTILESPEWRIGHT,
	GOLDTORIZOTURN,
	GOLDTORIZOJUMPBACKLEFT,
	GOLDTORIZOJUMPFORWARDLEFT,
	GOLDTORIZOJUMPBACKRIGHT,
	GOLDTORIZOJUMPFORWARDRIGHT,
	GOLDTORIZOWALKLEFTFLASHING,
	GOLDTORIZOWALKRIGHTFLASHING,
	GOLDTORIZOJUMPLEFTFLASHING,
	GOLDTORIZOJUMPRIGHTFLASHING
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

	bool m_BossComplete = false;
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
	int m_PlayerHealthOffset = 0;

	b2Body* body;

	sf::Vector2f position;
	virtual void createFixture() = 0;

	FixtureData* projectileDestroyed;
	b2Fixture* m_PlayerHitbox;
	bool m_IsPlayerInvulnerable;

	FixtureData fixtureData;
	b2Fixture* fixture;

public:
	bool destroyed = false;
	bool collided = false;

	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;

	// Getters and setters
	void setPlayerHitbox(b2Fixture* hitbox) { m_PlayerHitbox = hitbox; }
	void setPlayerinvulnerabillity(bool isInvulnerable) { m_IsPlayerInvulnerable = isInvulnerable; }
	FixtureData* const getProjectileDestroyed() { return projectileDestroyed; }
	int const getPlayerHealthOffset() { return m_PlayerHealthOffset; };
};

class Spore : public BossComponent
{
private:
	float m_SwitchTime = 0.01f;
	float m_TotalTime = 0.0f;

	std::vector<sf::Vector2f> m_SporeInitialPositions = {sf::Vector2f(3.0f, 2.3f), sf::Vector2f(6.0f, 2.3f) , sf::Vector2f(9.0f, 2.3f) , sf::Vector2f(12.0f, 2.3f) };
	
	void createFixture() override;

	std::vector<sf::Texture> m_SporeTextures;
	SheetlessAnimation* m_SporeAnimation;
public:
	~Spore();

	void begin(unsigned int positionIndex);
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

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

public:
	void begin() override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	void resetFixture() override;

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

class TorizoBomb : public BossComponent
{
private:
	void createFixture() override;

	bool m_CollidedWithMap;
	int m_ExplodeCounter = 0;

	sf::Vector2f m_BossPosition;
	Direction m_Orientation;

	SheetlessAnimation* m_BombDestructionAnim;
	SheetlessAnimation* m_BombAnim;

	b2FixtureDef m_FixtureDef{};
	b2Fixture* m_BombHitbox;
public:
	TorizoBomb(sf::Vector2f bossPosition);
	~TorizoBomb();

	void destroyFixture();

	SheetlessAnimation* currentSheetlessAnimation;

	void begin();
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	//Getters and setters
	void const setOrientation(Direction orientation) { m_Orientation = orientation; };

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

class TorizoArk : public BossComponent
{
private:
	void createFixture() override;

	SheetlessAnimation* m_ArkLeftAnim;
	SheetlessAnimation* m_ArkRightAnim;
	
	sf::Vector2f m_BossPosition;

	Direction m_Orientation;
	
public:
	TorizoArk(sf::Vector2f bossPosition);
	~TorizoArk();

	SheetlessAnimation* currentSheetlessAnimation;

	void begin();
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;

	//Getters and setters
	void const setOrientation(Direction orientation) { m_Orientation = orientation; };

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

class GoldTorizo : public Boss
{
private:
	void createFixture() override;
	void createActiveAnimations();

	void activateBombs();
	void activateArks();
	void attack(float deltaTime);
	
	void activateJump(bool shouldJumpLeft);
	b2Vec2 calculateJumpPosition(b2Vec2 startingPosition, b2Vec2 startingVelocity);

	unsigned int m_JumpTimeStep = 0;
	b2Vec2 m_StartingJumpPosition = b2Vec2(27.5f, 14.5f);
	b2Vec2 m_SamusJumpStartingPosition;
	int m_BossPlayerDistance;
	std::pair<float, float> m_RoomXDimensions = std::make_pair(2.0f, 30.0f);
	std::pair<float, float> m_BossWallDistance;
	bool m_ShouldJumpForward;

	Direction m_Orientation;
	std::queue<BossAnimationState> m_BossActions;

	bool m_IntroOver = false;
	bool m_IsHit = false;

	bool m_BombsActive = false;
	bool m_ArksActive = false;

	bool m_CanJumpLeft = false;
	bool m_CanJumpRight = false;
	
	bool m_Jumping = false;
	bool m_Attacking = false;

	TorizoBomb* m_Bomb;
	std::vector<TorizoBomb*> m_Bombs;

	TorizoArk* m_Ark;
	std::vector<TorizoArk*> m_Arks;

	float m_BombSwitchTime = 0.2f;
	float m_BombTotalTime = 0.2f;
	float m_BombTotalActivatonTime = 0.0f;
	float m_BombStopActivatonTime = 3.0f;

	float m_ArkSwitchTime = 0.75f;
	float m_ArkTotalTime = 0.75f;
	float m_ArkTotalActivatonTime = 0.0f;
	float m_ArkStopActivatonTime = 5.0f;

public:
	void begin() override;
	void update(float deltaTime);
	void draw(Renderer& renderer);

	void resetFixture();

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};

