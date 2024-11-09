#pragma once

#include <box2d/b2_polygon_shape.h>

#include "Physics.h"
#include "Renderer.h"
#include "Animation.h"
#include "Menu.h"

class Door : public Collisionlistener
{
private:
	void createFixture();
	void deleteClosedBody();

	sf::Vector2f m_Position;

	SheetlessAnimation* m_DoorOpenLeftAnimation;
	SheetlessAnimation* m_DoorOpenRightAnimation;
	bool m_IsDoorOpen;

	b2Body* m_OpenBody;
	b2Body* m_ClosedBody;
	b2Fixture* m_DoorClosedFixture;
	b2Fixture* m_DoorOpenFixture;
	FixtureData* m_FixtureData = new FixtureData();

	MenuState m_DoorLink;
	Direction m_Orientation;

	bool m_IsThroughDoor = false;
public:
	Door(MenuState doorLink = NOMENU, Direction Orientation = LEFT);
	
	void begin();
	void update(float deltaTime);
	void draw(Renderer& renderer);

	void destroyOpenBody();
	void destroyClosedBody();

	// Getters and setters
	void const setPosition(sf::Vector2f position) { m_Position = position; }

	void const setIsThroughDoor(bool isThroughDoor) { m_IsThroughDoor = isThroughDoor; }
	bool const getIsThroughDoor() { return m_IsThroughDoor; }

	MenuState const getDoorLink() { return m_DoorLink; }

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};