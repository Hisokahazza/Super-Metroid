#pragma once

#include <SFML/Graphics.hpp>

#include <box2d/b2_body.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include "Renderer.h"
#include "Physics.h"
#include "Resources.h"
#include "Animation.h"

enum Direction
{
	RIGHT,
	LEFT,
	UPRIGHT,
	UPLEFT,
	DOWNRIGHT,
	DOWNLEFT,
	UP,
	DOWN
};

enum ProjectileType
{
	BULLETPROJ,
	MISSILEPROJ,
	SUPERMISSILEPROJ
};

class Projectile : public Collisionlistener
{
private:
protected:
	b2Body* body;

	float projectileSpeed;
	std::string projectileTex;
	sf::Vector2f position{};

public:
	virtual void begin(b2Vec2 initialPosition) = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw(Renderer& renderer) = 0;

	bool destroyed = false;

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};


class DefaultBullet : public Projectile
{
private:
	Direction m_BulletDirection;
	void createFixture(b2Vec2 inititalPosition);

	SheetlessAnimation* m_BulletDestructionAnim;
	SheetlessAnimation* m_BulletAnim;

public:
	DefaultBullet(Direction bulletDirection);
	~DefaultBullet();

	FixtureData fixtureData{};
	b2Fixture* bulletFixture;

	SheetlessAnimation* currentSheetlessAnimation;

	void begin(b2Vec2 initialPosition);
	void update(float deltaTime);
	void draw(Renderer& renderer);
};

class Missile : public Projectile
{
private:
	Direction m_MissileDirection;
	b2Fixture* missileFixture;

	float m_MaxSpeed;
	void createFixture(b2Vec2 inititalPosition);

	SheetlessAnimation* m_MissileDestructionAnim;
public:
	Missile(Direction missileDirection);
	~Missile();

	SheetlessAnimation* currentSheetlessAnimation;

	FixtureData fixtureData{};

	void begin(b2Vec2 initialPosition);
	void update(float deltaTime);
	void draw(Renderer& renderer);
};