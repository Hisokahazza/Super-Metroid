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

// Enum for fixture types
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

// struct for all fixture data pointers
struct FixtureData
{
	// Each fixture has a listener and type
	Collisionlistener* listener;
	FixtureType type;

	bool isActive = true;

	// Union with pointers to collidable objects 
	// (union is only allocated as much memory as its largest member ensuring each fixture can only collide with a fixture of one other type at a time)
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
	static void update(float deltaTime);
	static void debugDraw(Renderer& renderer);

	static DebugDrawImp* debugDrawObj;
	static b2World world;
};

