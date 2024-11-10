#include "Projectile.h"

// Collision handling 
void Projectile::onBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;

	if (!otherData)
	{
		return;
	}
	// Handle collision with map or door
	if (otherData->type == MAPTILE || otherData->type == DOOR)
	{
		destroyed = true;
		selfData->isActive = false;
	}
}

void Projectile::onEndContact(b2Fixture* self, b2Fixture* other)
{
}

// Create bullet fixture
void DefaultBullet::createFixture(b2Vec2 initialPosition)
{
	fixtureData.type = BULLET;
	fixtureData.bullet = this;
	fixtureData.listener = this;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = initialPosition;
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2CircleShape circleShape{};
	circleShape.m_p.Set(0.0f, 0.0f);
	circleShape.m_radius = 0.1f;

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
	fixtureDef.shape = &circleShape;
	fixtureDef.isSensor = true;
	m_BulletFixture = body->CreateFixture(&fixtureDef);
}

DefaultBullet::DefaultBullet(Direction bulletDirection) : m_BulletDirection(bulletDirection)
{
}

// bullet destructor to be called when bullets are deleted
DefaultBullet::~DefaultBullet()
{
	currentSheetlessAnimation = m_BulletDestructionAnim;

	if (body)
	{
		// Destroy the fixture
		body->DestroyFixture(m_BulletFixture);

		// Destroy the body
		Physics::world.DestroyBody(body);

		// Set the body to nullptr to avoid using it accidentally later
		body = nullptr;  
	}
}

void DefaultBullet::begin(b2Vec2 initialPosition)
{
	// Create bullet fixture at position based on samus position
	createFixture(initialPosition);

	// Initialise sheetless animation textures
	std::vector <sf::Texture> bulletDestructionTextures = {
		Resources::textures["Default_Bullet.png"],
		Resources::textures["Default_Bullet_Destroy_01.png"],
		Resources::textures["Default_Bullet_Destroy_02.png"],
		Resources::textures["Default_Bullet_Destroy_03.png"],
		Resources::textures["Default_Bullet_Destroy_04.png"]
	};
	std::vector <sf::Texture> defaultBulletTextures = {
		Resources::textures["Default_Bullet.png"]
	};

	// Initialise sheetless animations
	m_BulletDestructionAnim = new SheetlessAnimation(bulletDestructionTextures, 0.1f, false, false, 1);
	m_BulletAnim = new SheetlessAnimation(defaultBulletTextures, 0.1f);

	currentSheetlessAnimation = m_BulletAnim;

	m_BulletAnim->begin();
	m_BulletDestructionAnim->begin();
}

void DefaultBullet::update(float deltaTime)
{
	// Get and set bullet velocity each frame
	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	projectileSpeed = 10.0f;

	// Check for direction determined in samus class and adjust bullet direction accordingly
	switch (m_BulletDirection)
	{
	case RIGHT:
		velocity.x += projectileSpeed;
		break;
	case LEFT:
		velocity.x -= projectileSpeed;
		break;
	case UPRIGHT:
		velocity.x += projectileSpeed / 2;
		velocity.y -= projectileSpeed / 1.5;
		break;
	case UPLEFT:
		velocity.x -= projectileSpeed / 2;
		velocity.y -= projectileSpeed / 1.5;
		break;
	case DOWNRIGHT:
		velocity.x += projectileSpeed / 2;
		velocity.y += projectileSpeed / 1.5;
		break;
	case DOWNLEFT:
		velocity.x -= projectileSpeed / 2;
		velocity.y += projectileSpeed / 1.5;
		break;
	case UP:
		velocity.y -= projectileSpeed;
		break;
	case DOWN:
		velocity.y += projectileSpeed;
		break;
	}
	
	// Set new velocity
	body->SetLinearVelocity(velocity);

	// Alter position variable each frame
	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);

	currentSheetlessAnimation->update(deltaTime);
}

void DefaultBullet::draw(Renderer& renderer)
{
	if (currentSheetlessAnimation == m_BulletAnim)
	{
		renderer.draw(currentSheetlessAnimation->getCurrentFrame(), position, sf::Vector2f(0.3f, 0.3f));
	}
	else if (currentSheetlessAnimation == m_BulletDestructionAnim)
	{
		renderer.draw(currentSheetlessAnimation->getCurrentFrame(), position, sf::Vector2f(0.6f, 0.6f));
	}
}

// Create missile fixture
void Missile::createFixture(b2Vec2 initialPosition)
{
	fixtureData.type = MISSILE;
	fixtureData.missile = this;
	fixtureData.listener = this;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = initialPosition;
	bodyDef.fixedRotation = true;
	body = Physics::world.CreateBody(&bodyDef);

	b2CircleShape circleShape{};
	circleShape.m_radius = 0.2f;
	circleShape.m_p.Set(0, 0);

	b2FixtureDef fixtureDef;
	fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
	fixtureDef.shape = &circleShape;
	fixtureDef.isSensor = true;
	missileFixture = body->CreateFixture(&fixtureDef);
}

Missile::Missile(Direction missileDirection) : m_MissileDirection(missileDirection)
{
}

// Missile destructor to be called when bullets are deleted
Missile::~Missile()
{
	currentSheetlessAnimation = m_MissileDestructionAnim;

	if (body)
	{
		// Destroy the fixture
		body->DestroyFixture(missileFixture);

		// Destroy the body
		Physics::world.DestroyBody(body);

		// Set the pointer to nullptr to avoid using it accidentally later
		body = nullptr;  
	}
}

void Missile::begin(b2Vec2 initialPosition)
{
	// Initialise sheetless animation textures
	std::vector <sf::Texture> missileDestructionTextures = 
	{
		Resources::textures["Missile_Destroy_01.png"],
		Resources::textures["Missile_Destroy_02.png"],
		Resources::textures["Missile_Destroy_03.png"],
		Resources::textures["Missile_Destroy_04.png"],
		Resources::textures["Missile_Destroy_05.png"],
		Resources::textures["Missile_Destroy_06.png"]

	};

	std::vector<sf::Vector2f> missileDestructionFrameSizes = 
	{
		sf::Vector2f(0.6f, 0.6f),
		sf::Vector2f(0.8f, 0.8f),
		sf::Vector2f(1.0f, 1.0f),
		sf::Vector2f(1.2f, 1.2f),
		sf::Vector2f(1.4f, 1.4f),
		sf::Vector2f(1.6f, 1.6f),
	};

	// Initialise sheetless animation
	m_MissileDestructionAnim = new SheetlessAnimation(missileDestructionTextures, 0.1f, false, false, 1, missileDestructionFrameSizes);
	m_MissileDestructionAnim->begin();

	createFixture(initialPosition);
	projectileSpeed = 0.5f;
}

void Missile::update(float deltaTime)
{
	// Handle variable missile velocity
	if (projectileSpeed <= m_MaxSpeed)
	{
		projectileSpeed *= 2.0f;
	}

	// Get and set velocity
	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x = 0.0f;
	velocity.y = 0.0f;

	// check for direction determined in samus class
	switch (m_MissileDirection)
	{
	case RIGHT:
		velocity.x += projectileSpeed;
		break;
	case LEFT:
		velocity.x -= projectileSpeed;
		break;
	case UPRIGHT:
		velocity.x += projectileSpeed / 2;
		velocity.y -= projectileSpeed / 1.5;
		break;
	case UPLEFT:
		velocity.x -= projectileSpeed / 2;
		velocity.y -= projectileSpeed / 1.5;
		break;
	case DOWNRIGHT:
		velocity.x += projectileSpeed / 2;
		velocity.y += projectileSpeed / 1.5;
		break;
	case DOWNLEFT:
		velocity.x -= projectileSpeed / 2;
		velocity.y += projectileSpeed / 1.5;
		break;
	case UP:
		velocity.y -= projectileSpeed;
		break;
	case DOWN:
		velocity.y += projectileSpeed;
		break;
	}

	// Set determined velocity
	body->SetLinearVelocity(velocity);

	// Alter position each frame
	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
}

void Missile::draw(Renderer& renderer)
{
	// Handle missile rendering dependent on missile orientation
	switch (m_MissileDirection)
	{
	case RIGHT:
		renderer.draw(Resources::textures["Missile_Right.png"], position, sf::Vector2f(0.5f, 0.3f));
		break;
	case LEFT:
		renderer.draw(Resources::textures["Missile_Left.png"], position, sf::Vector2f(0.5f, 0.3f));
		break;
	case UP:
		renderer.draw(Resources::textures["Missile_Up.png"], position, sf::Vector2f(0.3f, 0.5f));
		break;
	case UPRIGHT:
		renderer.draw(Resources::textures["Missile_Up.png"], position, sf::Vector2f(0.3f, 0.5f), 45.0f);
		break;
	case UPLEFT:
		renderer.draw(Resources::textures["Missile_Up.png"], position, sf::Vector2f(0.3f, 0.5f), 315.0f);
		break;
	case DOWNRIGHT:
		renderer.draw(Resources::textures["Missile_Up.png"], position, sf::Vector2f(0.3f, 0.5f), 135.0f);
		break;
	case DOWNLEFT:
		renderer.draw(Resources::textures["Missile_Up.png"], position, sf::Vector2f(0.3f, 0.5f), 225.0f);
		break;
	}

	if (destroyed == true)
	{
		renderer.draw(currentSheetlessAnimation->getCurrentFrame(), position, currentSheetlessAnimation->getCurrentFrameSize());
	}
}
