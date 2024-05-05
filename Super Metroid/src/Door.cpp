#include "Door.h"

void Door::createFixture()
{
	b2BodyDef bodyDef{};
	bodyDef.position.Set();
	b2Body* body = Physics::world.CreateBody(&bodyDef);

	b2PolygonShape shape{};
	shape.SetAsBox(1.0f, 1.5f);

	FixtureData* fixtureData = new FixtureData();

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)fixtureData;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
}

void Door::Begin()
{
}

void Door::Update(float deltaTime)
{
}

void Door::Draw(Renderer& renderer)
{
}
