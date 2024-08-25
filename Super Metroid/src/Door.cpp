#include "Door.h"

void Door::createFixture()
{
	// Create closed door body
	m_FixtureData->door = this;
	m_FixtureData->type = DOOR;
	m_FixtureData->listener = this;

	b2BodyDef closedbodyDef{};
	closedbodyDef.position.Set(m_Position.x, m_Position.y);
	closedbodyDef.type = b2_staticBody;
	m_ClosedBody = Physics::world.CreateBody(&closedbodyDef);

	b2PolygonShape shape{};
	shape.SetAsBox(0.75f, 1.5f);

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)m_FixtureData;
	fixtureDef.shape = &shape;
	fixtureDef.density = 0.0f;
	fixtureDef.friction = 0.0f;
	m_DoorClosedFixture = m_ClosedBody->CreateFixture(&fixtureDef);

	// Create open door body
	b2BodyDef openbodyDef{};
	openbodyDef.position.Set(m_Position.x - 0.2f, m_Position.y);
	openbodyDef.type = b2_staticBody;
	m_OpenBody = Physics::world.CreateBody(&openbodyDef);

	shape.SetAsBox(0.4f, 1.4f);
	m_DoorOpenFixture = m_OpenBody->CreateFixture(&fixtureDef);
}

void Door::deleteClosedBody()
{
	// Destroy closed door body
	if (m_ClosedBody)
	{
		m_ClosedBody->DestroyFixture(m_DoorClosedFixture);

		Physics::world.DestroyBody(m_ClosedBody);

		m_ClosedBody = nullptr;
	}
}

Door::Door(MenuState doorLink, Direction Orientation) : m_DoorLink(doorLink), m_Orientation(Orientation)
{
}

void Door::destroyFixture()
{
	if (m_OpenBody)
	{
		m_OpenBody->DestroyFixture(m_DoorOpenFixture);

		Physics::world.DestroyBody(m_OpenBody);

		m_OpenBody = nullptr;
	}
}

void Door::Begin()
{
	createFixture();

	// Initialise door textures 
	std::vector<sf::Texture> doorOpenLeftTextures
	{
		Resources::textures["HUB_Door_L_01.png"],
		Resources::textures["HUB_Door_L_02.png"],
		Resources::textures["HUB_Door_L_03.png"],
		Resources::textures["HUB_Door_L_04.png"],
		Resources::textures["HUB_Door_L_05.png"]
	};
	std::vector<sf::Texture> doorOpenRightTextures
	{
		Resources::textures["HUB_Door_R_01.png"],
		Resources::textures["HUB_Door_R_02.png"],
		Resources::textures["HUB_Door_R_03.png"],
		Resources::textures["HUB_Door_R_04.png"],
		Resources::textures["HUB_Door_R_05.png"]
	};

	m_DoorOpenLeftAnimation = new SheetlessAnimation(doorOpenLeftTextures, 0.1f, false, false, 1);
	m_DoorOpenRightAnimation = new SheetlessAnimation(doorOpenRightTextures, 0.1f, false, false, 1);

	m_DoorOpenLeftAnimation->begin();
	m_DoorOpenRightAnimation->begin();
}

void Door::Update(float deltaTime)
{
	// Handle door animation updating and door destruction 
	if (m_IsDoorOpen == true)
	{
		if (m_Orientation == LEFT)
		{
			m_DoorOpenLeftAnimation->update(deltaTime);

			if (m_DoorOpenLeftAnimation->checkPlaying() == false)
			{
				deleteClosedBody();
			}
		}
		else if (m_Orientation == RIGHT)
		{
			m_DoorOpenRightAnimation->update(deltaTime);

			if (m_DoorOpenRightAnimation->checkPlaying() == false)
			{
				deleteClosedBody();
			}
		}
		
	}
}

void Door::draw(Renderer& renderer)
{
	// Handle door draw calls
	if (m_IsDoorOpen == false)
	{
		if (m_Orientation == LEFT)
		{
			renderer.draw(Resources::textures["HUB_Door_L_01.png"], m_Position, sf::Vector2f(1.5f, 3.0f));
		}
		else if (m_Orientation == RIGHT)
		{
			renderer.draw(Resources::textures["HUB_Door_R_01.png"], m_Position, sf::Vector2f(1.5f, 3.0f));
		}
	}
	else if (m_IsDoorOpen == true)
	{
		if (m_Orientation == LEFT)
		{
			renderer.draw(m_DoorOpenLeftAnimation->getCurrentFrame(), m_Position, sf::Vector2f(1.5f, 3.0f));
		}
		if (m_Orientation == RIGHT)
		{
			renderer.draw(m_DoorOpenRightAnimation->getCurrentFrame(), m_Position, sf::Vector2f(1.5f, 3.0f));
		}
	}
}

void Door::onBeginContact(b2Fixture* self, b2Fixture* other)
{
	// Handle door collisions
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;

	if (!otherData)
	{
		return;
	}

	if (otherData->type == BULLET || otherData->type == MISSILE)
	{
		m_IsDoorOpen = true;
	}

	if (self == m_DoorOpenFixture && otherData->type == SAMUS && m_IsDoorOpen == true)
	{
		m_IsThroughDoor = true;
		m_IsDoorOpen = false;
	}
}

void Door::onEndContact(b2Fixture* self, b2Fixture* other)
{
}
