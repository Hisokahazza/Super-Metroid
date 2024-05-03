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

		// Set the pointer to nullptr to avoid using it accidentally later
		body = nullptr;
	}
}

void Spore::begin(unsigned int positionIndex)
{
	// Set initial spore position based on response from sporespawn class
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

		// Simulate a sin wave rotated 180 degrees for spore movement
		position.y += 0.025f;
		position.x += sin(position.y) / 50;
		body->SetTransform(b2Vec2(position.x, position.y), 0.0f);
	}

	// Ensure uncollided spore objects are destroyed
	if (position.y > 20.0f)
	{
		destroyed = true;
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

	// Handle spore collision with samus
	if (otherData->type == SAMUS && m_PlayerHitbox == other && m_IsPlayerInvulnerable == false)
	{
		destroyed = true;
		selfData->isActive = false;
		m_PlayerHealthOffset = -20;
		collided = true;
	}
	// Handle spore collision with bullets and missiles
	else if (otherData->type == BULLET || otherData->type == MISSILE)
	{
		destroyed = true;
		selfData->isActive = false;

		projectileDestroyed = otherData;
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
	circleShape.m_radius = 2.0f;
	circleShape.m_p.Set(0, 0);
	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&m_FixtureData;
	fixtureDef.isSensor = true;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 0.0f;
	m_CoreClosed = body->CreateFixture(&fixtureDef);

	b2PolygonShape polygonShape{};
	polygonShape.SetAsBox(0.4f, 1.3f);
	fixtureDef.shape = &polygonShape;
	m_CoreFixture = body->CreateFixture(&fixtureDef);

	// Create bottom and top fixtures for when core is open
	polygonShape.SetAsBox(1.7f, 0.65f, b2Vec2(0.0f, -1.45f), 0.0f);
	m_CoreOpenTop = body->CreateFixture(&fixtureDef);

	polygonShape.SetAsBox(1.7f, 0.65f, b2Vec2(0.0f, 2.2f), 0.0f);
	m_CoreOpenBottom = body->CreateFixture(&fixtureDef);

	// create collidable body so player doesn't overlap with the boss
	circleShape.m_radius = 1.5f;
	circleShape.m_p.Set(0, 0);
	fixtureDef.isSensor = false;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 0.0f;
	body->CreateFixture(&fixtureDef);
}

void SporeSpawn::closeCore(float deltaTime)
{
	m_CoreTotalTime += deltaTime;

	if (m_CoreTotalTime >= m_CoreClosedSwitchTime)
	{
		m_CoreTotalTime -= m_CoreClosedSwitchTime;
		m_IsCoreOpen = false;
		m_IsCoreHit = false;
		m_IsCoreClosing = true;
	}
}

void SporeSpawn::openCore(float deltaTime)
{
	m_CoreTotalTime += deltaTime;

	if (m_CoreTotalTime >= m_CoreOpenSwitchTime)
	{
		m_CoreTotalTime -= m_CoreOpenSwitchTime;
		m_IsCoreOpen = true;
		m_IsCoreClosing = false;
	}
}

void SporeSpawn::createActiveAnimations()
{
	m_ActiveStates = { COREOPENING, COREOPENED, CORECLOSING, CORECLOSED, COREFLASHING };

	// Initialise textures for sheetless Animations
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
	std::vector<sf::Texture> coreClosedTextures = {
		Resources::textures["SSP_Idle.png"]
	};
	std::vector<sf::Texture> coreFlashingTextures = {
		Resources::textures["SSP_Open_Flash_01.png"],
		Resources::textures["SSP_Open_Flash_02.png"],
		Resources::textures["SSP_Open_Flash_03.png"],
		Resources::textures["SSP_Open_Flash_02.png"]
	};

	// Initialise sheetless animations
	m_SheetlessAnimations[COREOPENING] = new SheetlessAnimation(coreOpeningTextures, 0.2f, false, 1);
	m_SheetlessAnimations[COREOPENED] = new SheetlessAnimation(coreOpenedTextures, 0.2f, false);
	m_SheetlessAnimations[CORECLOSING] = new SheetlessAnimation(coreOpeningTextures, 0.1f, false, 1, true);
	m_SheetlessAnimations[CORECLOSED] = new SheetlessAnimation(coreClosedTextures);
	m_SheetlessAnimations[COREFLASHING] = new SheetlessAnimation(coreFlashingTextures, 0.1f, false, 2);
}

sf::Vector2f SporeSpawn::determineSegmentPos(float positionOnLine)
{
	// Calculate the gradient of the line between the tether point and the boss
	float gradient = (m_TetherPoint.y - position.y) / (m_TetherPoint.x - position.x);

	// Calculate the y-intercept of the line
	float yIntercept = position.y - (gradient * position.x);

	// Calculate the range between the x-coordinates of the tether point and the boss
	float range = m_TetherPoint.x - position.x;

	// Calculate the x-coordinate of the segment position on the line
	float segmentX = position.x + (range / positionOnLine);

	// Calculate the y-coordinate of the segment position on the line
	float segmentY = (gradient * segmentX) + yIntercept;

	// Return the calculated segment position
	return sf::Vector2f(segmentX, segmentY);
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
	// Reset timer when core is hit 
	if (m_IsCoreHit == true)
	{
		m_CoreTotalTime = 0;
	}

	// Set base Animation state to core closed
	m_CurrentAnimationState = CORECLOSED;

	// Check if core is open
	if (m_IsCoreOpen == true)
	{
		m_CurrentAnimationState = COREOPENING;

		if (m_SheetlessAnimations[COREOPENING]->checkPlaying() == false)
		{
			m_CurrentAnimationState = COREOPENED;
			m_IsHittable = true;
		}
	}
	// Check if core is closing to play closing Animation
	else if (m_IsCoreClosing == true)
	{
		m_IsHittable = false;

		if (m_IsCoreHit == true)
		{
			m_CurrentAnimationState = COREFLASHING;
		}
		else if (m_IsCoreHit == false)
		{
			m_CurrentAnimationState = CORECLOSING;

			if (m_SheetlessAnimations[CORECLOSING]->checkPlaying() == false)
			{
				m_IsCoreOpen = false;
				m_IsCoreClosing = false;
			}
		}

		if (m_SheetlessAnimations[COREFLASHING]->checkPlaying() == false)
		{
			m_CurrentAnimationState = CORECLOSING;
			if (m_SheetlessAnimations[CORECLOSING]->checkPlaying() == false)
			{
					m_IsCoreClosing = false;
					m_IsCoreHit = false;
			}
		}
	}

	// Close and open core based on a switch time
	if (m_IsCoreOpen == true)
	{
		closeCore(deltaTime);
	}
	if (m_IsCoreOpen == false)
	{
		openCore(deltaTime);
	}



	// Update and check for collisions of spore objects
	for (auto spore : m_Spores)
	{
		if (spore->collided == true)
		{
			playerHealthOffset += spore->getPlayerHealthOffset();
			spore->collided = false;
		}

		if (spore->getProjectileDestroyed() != nullptr)
		{
			projectileDestroyed = spore->getProjectileDestroyed();
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
			spore->setPlayerinvulnerabillity(isPlayerInvulnerable);
		}
	}

	if (menuManager.menus[menuManager.getSwitchScreen()]->returnToHub == true)
	{
		std::cout << "SPORE" << std::endl;
		for (auto& spore : m_Spores)
		{
			spore->~Spore();
		}
	}

	// Handle Spore Spawning
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

	switch (attributes.health)
	{
	case 800:
		m_BossSpeed = 4;
		break;
	case 600:
		m_BossSpeed = 5;
		break;
	case 0:
		m_BossComplete = true;
		break;
	}

	// Handle boss movement
	if (m_BossComplete == false)
	{
		for (int count = 0; count < m_BossSpeed; count++)
		{
			// Change boss direction if it reaches the bounds of the arena
			if (position.x >= 12.7f)
			{
				m_BossChangedDirection = true;
			}
			else if (position.x <= 2.3f)
			{
				m_BossChangedDirection = false;
			}

			// Simulate sin wave for boss movement
			if (m_BossChangedDirection == false && m_IsCoreOpen == false)
			{
				position.x += 0.01f;
				position.y += sin(position.x) / 30;
				body->SetTransform(b2Vec2(position.x, position.y), 0.0f);
			}
			else if (m_BossChangedDirection == true && m_IsCoreOpen == false)
			{
				position.x -= 0.01f;
				position.y -= sin(position.x) / 30;
				body->SetTransform(b2Vec2(position.x, position.y), 0.0f);
			}
		}
	}
	
	// Handle Animation resetting
	if (m_CurrentAnimationState == CORECLOSED)
	{
		for (auto state : m_ActiveStates)
		{
			// When in idle state reset all other states
			if (state != m_CurrentAnimationState)
			{
				m_SheetlessAnimations[state]->reset();
			}
		}
	}

	if (m_BossComplete == true)
	{
		m_CurrentAnimationState = CORECLOSING;
		if (m_SheetlessAnimations[CORECLOSING]->checkPlaying() == false)
		{
			m_CurrentAnimationState = CORECLOSED;
		}
	}

	if (menuManager.getSwitchScreen() == NOMENU)
	{
		m_SheetlessAnimations[m_CurrentAnimationState]->update(deltaTime);
	}
	
	if (m_BossComplete == true && m_CurrentAnimationState == CORECLOSED)
	{
		menuManager.setSwitchScreen(VICTORY);
		m_BossComplete = false;
	}
}

void SporeSpawn::draw(Renderer& renderer)
{
	for (float count = 1; count <= 1.8f; count += 0.4f)
	{
		renderer.draw(Resources::textures["SSP_Segment.png"], determineSegmentPos(count), sf::Vector2f(1.0f, 1.0f));
	}

	renderer.draw(m_SheetlessAnimations[m_CurrentAnimationState]->getCurrentFrame(), position, sf::Vector2f(4.0f, 6.0f));

	for (auto spore : m_Spores)
	{
		spore->draw(renderer);
	}
}

void SporeSpawn::resetFixture()
{
	if (body)
	{
		b2Fixture* fixturesToDelete = body->GetFixtureList();
		body->DestroyFixture(fixturesToDelete);

		Physics::world.DestroyBody(body);
	}
}

void SporeSpawn::onBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;

	// Handle boss collisions with missiles when vulnerable
	if (m_CoreFixture == self && otherData->type == MISSILE && m_IsCoreOpen == true)
	{
		attributes.health -= 200;
		m_IsCoreOpen = false;
		m_IsCoreClosing = true;
		m_IsCoreHit = true;
		projectileDestroyed = otherData;
	}
	else if (m_CoreFixture == self && otherData->type == BULLET)
	{
		projectileDestroyed = otherData;
	}

	// Handle boss collision with player
	if (other == playerHitbox && otherData->type == SAMUS && isPlayerInvulnerable == false)
	{
		if (m_CurrentAnimationState == CORECLOSED && self == m_CoreClosed)
		{
			std::cout << "COLLIDED" << std::endl;
			playerHealthOffset -= 20;
			m_IsSamusHit = true;
		}
		else if (m_CurrentAnimationState != CORECLOSED && (self == m_CoreOpenBottom || self == m_CoreOpenTop || self == m_CoreFixture))
		{
			playerHealthOffset -= 20;
			m_IsSamusHit = true;
		}
	}

	// Handle boss collisions with missiles and bullets when invulnerable
	if (otherData->type == BULLET || otherData->type == MISSILE)
	{
		if (m_CurrentAnimationState == CORECLOSED)
		{
			if (self == m_CoreClosed)
			{
				projectileDestroyed = otherData;
			}
		}
		else if (m_CurrentAnimationState != CORECLOSED)
		{
			if (self == m_CoreOpenTop || self == m_CoreOpenBottom)
			projectileDestroyed = otherData;
		}
	}
}

void SporeSpawn::onEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;
}

void GoldTorizo::createFixture()
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
	circleShape.m_radius = 2.0f;
	circleShape.m_p.Set(0, 0);
	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&m_FixtureData;
	fixtureDef.shape = &circleShape;
	body->CreateFixture(&fixtureDef);
}

void GoldTorizo::createActiveAnimations()
{
	m_ActiveStates = { GOLDTORIZOBLINK, GOLDTORIZOSTAND };

	// Initialise textures for sheetless Animations
	std::vector<sf::Texture> blinkTextures
	{
		Resources::textures["GT_Intro_01.png"],
		Resources::textures["GT_Intro_02.png"],
		Resources::textures["GT_Intro_03.png"],
		Resources::textures["GT_Intro_04.png"],
	};
	std::vector<sf::Texture> standTextures
	{
		Resources::textures["GT_Intro_05.png"],
		Resources::textures["GT_Intro_06.png"],
		Resources::textures["GT_Intro_07.png"],
		Resources::textures["GT_Intro_08.png"],
		Resources::textures["GT_Intro_09.png"],
		Resources::textures["GT_Intro_10.png"]
	};

	// Initialise sheetless animations
	m_SheetlessAnimations[GOLDTORIZOBLINK] = new SheetlessAnimation(blinkTextures, 0.2f, true, 5);
	m_SheetlessAnimations[GOLDTORIZOSTAND] = new SheetlessAnimation(standTextures, 0.1f, false, 1);
}

void GoldTorizo::begin()
{
	createFixture();
	createActiveAnimations();

	for (auto state : m_ActiveStates)
	{
		m_SheetlessAnimations[state]->begin();
	}

	m_CurrentAnimationState = GOLDTORIZOBLINK;
}

void GoldTorizo::update(float deltaTime)
{
	if (m_SheetlessAnimations[m_CurrentAnimationState]->checkPlaying() == false)
	{
		m_CurrentAnimationState = GOLDTORIZOSTAND;
	}

	m_SheetlessAnimations[m_CurrentAnimationState]->update(deltaTime);
}

void GoldTorizo::draw(Renderer& renderer)
{
	renderer.draw(m_SheetlessAnimations[m_CurrentAnimationState]->getCurrentFrame(), sf::Vector2f(position.x, position.y - 0.35f), sf::Vector2f(3.5f, 4.0f));
}

void GoldTorizo::resetFixture()
{
}

void GoldTorizo::onBeginContact(b2Fixture* self, b2Fixture* other)
{
}

void GoldTorizo::onEndContact(b2Fixture* self, b2Fixture* other)
{
}
