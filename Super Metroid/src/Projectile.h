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


enum ProjectileType
{
	BULLETPROJ,
	MISSILEPROJ
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

	FixtureData fixtureData{};
	SheetlessAnimation* currentSheetlessAnimation;

	bool destroyed = false;

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};


class DefaultBullet : public Projectile
{
private:
	Direction m_BulletDirection;
	b2Fixture* m_BulletFixture;

	void createFixture(b2Vec2 inititalPosition);

	SheetlessAnimation* m_BulletDestructionAnim;
	SheetlessAnimation* m_BulletAnim;

public:
	DefaultBullet(Direction bulletDirection);
	~DefaultBullet();

	void begin(b2Vec2 initialPosition) override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};

class Missile : public Projectile
{
private:
	Direction m_MissileDirection;
	b2Fixture* missileFixture;

	float const m_MaxSpeed = 15.0f;
	void createFixture(b2Vec2 inititalPosition);

	SheetlessAnimation* m_MissileDestructionAnim;
public:
	Missile(Direction missileDirection);
	~Missile();

	void begin(b2Vec2 initialPosition) override;
	void update(float deltaTime) override;
	void draw(Renderer& renderer) override;
};