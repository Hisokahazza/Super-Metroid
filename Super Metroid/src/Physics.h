#pragma once

#include <box2d/b2_world.h>
#include <box2d/b2_world_callbacks.h>
#include <box2d/b2_contact.h>
#include <box2d/b2_draw.h>

#include "Renderer.h"

// Forward declare listener classes
class BossComponent;
class Samus;
class DefaultBullet;
class Missile;
class Boss;
class Door;


class Collisionlistener
{
private:
public:
	virtual void onBeginContact(b2Fixture* self, b2Fixture* other) = 0;
	virtual void onEndContact(b2Fixture* self, b2Fixture* other) = 0;
};

enum FixtureType
{
	MAPTILE,
	SAMUS,
	BULLET,
	MISSILE,
	BOSS,
	BOSSCOMPONENT,
	DOOR
};

struct FixtureData
{
	Collisionlistener* listener;
	FixtureType type;

	bool isActive = true;

	union
	{
		Samus* samus;
		DefaultBullet* bullet;
		Missile* missile;
		Boss* boss;
		BossComponent* bossComponent;
		Door* door;

		struct
		{
			int mapPosx, mapPosY;
		};
	};
};

class DebugDrawImp;

class Physics
{
private:
public:
	static void init();
	static void update(float deltaTime);
	static void debugDraw(Renderer& renderer);

	static DebugDrawImp* debugDrawObj;
	static b2World world;
};

