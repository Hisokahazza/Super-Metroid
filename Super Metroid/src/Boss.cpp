#include "Boss.h"

void Spore::createFixture()
{
	m_SporeFixtureData.type = BOSSCOMPONENT;
	m_SporeFixtureData.bossComponent = this;
	m_SporeFixtureData.listener = this;
	
	b2BodyDef bodyDef{};
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = b2Vec2(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2CircleShape circleShape{};
	circleShape.m_radius = 0.25f;
	circleShape.m_p.Set(0, 0);

	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&m_SporeFixtureData;
	fixtureDef.shape = &circleShape;
	fixtureDef.isSensor = true;
	m_SporeFixture = body->CreateFixture(&fixtureDef);
}

Spore::~Spore()
{
	if (body)
	{
		// Destroy the fixture
		body->DestroyFixture(m_SporeFixture);

		// Destroy the body
		Physics::world.DestroyBody(body);

		body = nullptr;  // Set the pointer to nullptr to avoid using it accidentally later
	}
}

void Spore::begin(unsigned int positionIndex)
{
	
	position = m_SporeInitialPositions[positionIndex];
	createFixture();

	m_SporeFrames = { Resources::textures["SSP_Spore_Proj_01.png"], Resources::textures["SSP_Spore_Proj_02.png"], Resources::textures["SSP_Spore_Proj_03.png"]};
	m_SporeAnimation = new SheetlessAnimation(m_SporeFrames, 0.1f);

	m_SporeAnimation->begin();
}

void Spore::update(float deltaTime)
{
	m_TotalTime += deltaTime;

	if (m_TotalTime >= m_SwitchTime)
	{
		m_TotalTime -= m_SwitchTime;

		position.y += 0.025f;
		position.x += sin(position.y) / 50;
		body->SetTransform(b2Vec2(position.x, position.y), 0.0f);
	}

	m_SporeAnimation->update(deltaTime);
}

void Spore::draw(Renderer& renderer)
{
	renderer.draw(m_SporeAnimation->getCurrentFrame(), position, sf::Vector2f(0.5f, 0.5f));
}

void Spore::onBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;

	if (!otherData)
	{
		return;
	}

	if (otherData->type == SAMUS && m_PlayerHitbox == other)
	{
		destroyed = true;
		selfData->isActive = false;
		m_PlayerHealthOffset = -20;
		collided = true;
	}
	else if (otherData->type == BULLET || otherData->type == MISSILE)
	{
		destroyed = true;
		selfData->isActive = false;
	}
}

void Spore::onEndContact(b2Fixture* self, b2Fixture* other)
{
}

void SporeSpawn::createFixture()
{
	m_FixtureData.listener = this;
	m_FixtureData.type = BOSS;
	m_FixtureData.boss = this;

	// Create boss body
	b2BodyDef bodyDef{};
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2CircleShape circleShape{};
	circleShape.m_radius = 1.5f;
	circleShape.m_p.Set(0, 0);
	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&m_FixtureData;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);

	b2PolygonShape polygonShape{};
	polygonShape.SetAsBox(0.3f, 1.0f);
	fixtureDef.shape = &polygonShape;
	fixtureDef.isSensor = true;
	m_CoreFixture = body->CreateFixture(&fixtureDef);

	// Create bottom and top fixtures for when core is open
	polygonShape.SetAsBox(1.3f, 0.5f, b2Vec2(0.0f, -1.1f), 0.0f);
	m_CoreOpenTop = body->CreateFixture(&fixtureDef);

	polygonShape.SetAsBox(1.3f, 0.5f, b2Vec2(0.0f, 1.65f), 0.0f);
	m_CoreOpenBottom = body->CreateFixture(&fixtureDef);
}

void SporeSpawn::createActiveAnimations()
{
	m_ActiveStates = { COREOPENING, COREOPENED, CORECLOSING, COREFLASHING };

	std::vector<sf::Texture> coreOpeningTextures = {
		Resources::textures["SSP_Idle.png"],
		Resources::textures["SSP_Anim_01.png"],
		Resources::textures["SSP_Anim_02.png"],
		Resources::textures["SSP_Anim_03.png"],
		Resources::textures["SSP_Anim_04.png"],
		Resources::textures["SSP_Anim_05.png"],
		Resources::textures["SSP_Anim_06.png"]
	};
	std::vector<sf::Texture> coreOpenedTextures = {
		Resources::textures["SSP_Open_01.png"],
		Resources::textures["SSP_Open_02.png"],
		Resources::textures["SSP_Open_03.png"]
	};
	std::vector<sf::Texture> coreFlashingTextures = {
		Resources::textures["SSP_Open_Flash_01.png"],
		Resources::textures["SSP_Open_Flash_02.png"],
		Resources::textures["SSP_Open_Flash_03.png"],
		Resources::textures["SSP_Open_Flash_02.png"]
	};

	m_SheetlessAnimations[COREOPENING] = new SheetlessAnimation(coreOpeningTextures, 0.2f, false, 1);
	m_SheetlessAnimations[COREOPENED] = new SheetlessAnimation(coreOpenedTextures, 0.2f, false);
	m_SheetlessAnimations[CORECLOSING] = new SheetlessAnimation(coreOpeningTextures, 0.1f, false, 1, true);
	m_SheetlessAnimations[COREFLASHING] = new SheetlessAnimation(coreFlashingTextures, 0.1f, false, 2);
}

void SporeSpawn::begin()
{
	createFixture();

	attributes = { 1000, 1 };

	createActiveAnimations();

	for (auto state : m_ActiveStates)
	{
		m_SheetlessAnimations[state]->begin();
	}
}

void SporeSpawn::update(float deltaTime)
{	
	if (m_CoreOpen == true)
	{
		m_CurrentAnimationState = COREOPENED;
		m_Hittable = true;
	}
	else
	{
		m_Hittable = false;
		m_CurrentAnimationState = COREFLASHING;

		if (m_SheetlessAnimations[COREFLASHING]->checkPlaying() == false)
		{
			m_CurrentAnimationState = CORECLOSING;
		}
	}

	for (auto spore : m_Spores)
	{
		if (spore->collided == true)
		{
			playerHealthOffset += spore->getPlayerHealthOffset();
			spore->collided = false;
		}

		if (spore->destroyed == true)
		{
			m_Spores.erase(std::find(m_Spores.begin(), m_Spores.end(), spore));
			spore->~Spore();
			delete spore;
		}
		else
		{
			spore->update(deltaTime);
			spore->setPlayerHitbox(playerHitbox);
		}
	}

	m_SporeTotalTime += deltaTime;

	if (m_SporeTotalTime >= m_SporeSwitchTime)
	{
		if (m_SpawnPosIndexSpore == 3)
		{
			m_SpawnPosIndexSpore = 0;
		}
		else
		{
			m_SpawnPosIndexSpore += 1;
		}

		m_SporeTotalTime -= m_SporeSwitchTime;

		m_Spore = new Spore();
		m_Spores.push_back(m_Spore);
		m_Spore->begin(m_SpawnPosIndexSpore);
	}

	for (int count = 0; count < m_BossSpeed; count++)
	{
		if (position.x >= 12.7f)
		{
			m_BossChangedDirection = true;
		}
		else if (position.x <= 2.3f)
		{
			m_BossChangedDirection = false;
		}

		if (m_BossChangedDirection == false && m_CoreOpen == false)
		{
			position.x += 0.01f;
			position.y += sin(position.x) / 30;
			body->SetTransform(b2Vec2(position.x, position.y), 0.0f);
		}
		else if (m_BossChangedDirection == true && m_CoreOpen == false)
		{
			position.x -= 0.01f;
			position.y -= sin(position.x) / 30;
			body->SetTransform(b2Vec2(position.x, position.y), 0.0f);
		}
	}

	m_SheetlessAnimations[m_CurrentAnimationState]->update(deltaTime);
}

void SporeSpawn::draw(Renderer& renderer)
{
	for (int count = 0; count <= 3; count++)
	{
		//renderer.draw(Resources::textures["SSP_Segment.png"], sf::Vector2f((position.x / count) + 6.0f, (position.y / count * 2) + 6.0f), sf::Vector2f(1.0f, 1.0f));
	}

	renderer.draw(m_SheetlessAnimations[m_CurrentAnimationState]->getCurrentFrame(), position, sf::Vector2f(3.0f, 4.5f));

	for (auto spore : m_Spores)
	{
		spore->draw(renderer);
	}
}

void SporeSpawn::onBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;

	if (m_CoreFixture == self && otherData->type == MISSILE)
	{
		attributes.health -= 200;
		m_CoreOpen = false;
		m_ProjectileDestroyed = otherData;
	}
	else if (m_CoreFixture == self && otherData->type == BULLET)
	{
		m_ProjectileDestroyed = otherData;
	}

	if (otherData->type == SAMUS)
	{
		playerHealthOffset -= 20;
	}
	if (otherData->type == BULLET || otherData->type == MISSILE)
	{
		if (m_CoreOpen == true)
		{
			if (self == m_CoreOpenTop || self == m_CoreOpenBottom)
			{
				m_ProjectileDestroyed = otherData;
			}
		}
		else if (m_CoreOpen == false)
		{
			m_ProjectileDestroyed = otherData;
		}
	}
}

void SporeSpawn::onEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;
}

