#include "Projectile.h"

#include <iostream>

void Projectile::onBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;

	if (!otherData)
	{
		return;
	}

	if (otherData->type == MAPTILE)
	{
		destroyed = true;
		selfData->isActive = false;
	}
}

void Projectile::onEndContact(b2Fixture* self, b2Fixture* other)
{
}

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
	bulletFixture = body->CreateFixture(&fixtureDef);
}

DefaultBullet::DefaultBullet(Direction bulletDirection) : m_BulletDirection(bulletDirection)
{
}

DefaultBullet::~DefaultBullet()
{
	m_CurrentSheetlessAnimation = m_BulletDestruction;

	if (body)
	{
		// Destroy the fixture
		body->DestroyFixture(bulletFixture);

		// Destroy the body
		Physics::world.DestroyBody(body);

		// Set the body to nullptr to avoid using it accidentally later
		body = nullptr;  
	}
}

// Create default bullet fixture
void DefaultBullet::begin(b2Vec2 initialPosition)
{
	createFixture(initialPosition);

	std::vector <sf::Texture> bulletDestructionTextures = {
		Resources::textures["Default_Bullet_Destroy_01.png"],
		Resources::textures["Default_Bullet_Destroy_02.png"],
		Resources::textures["Default_Bullet_Destroy_03.png"],
		Resources::textures["Default_Bullet_Destroy_04.png"]
	};
	std::vector <sf::Texture> defaultBulletTextures = {
		Resources::textures["Default_Bullet.png"]
	};

	m_BulletDestruction = new SheetlessAnimation(bulletDestructionTextures, 0.1f, false, 1);
	m_Bullet = new SheetlessAnimation(defaultBulletTextures, 0.1f);

	m_CurrentSheetlessAnimation = m_Bullet;

	m_CurrentSheetlessAnimation->begin();
}

void DefaultBullet::update(float deltaTime)
{
	b2Vec2 velocity = body->GetLinearVelocity();
	velocity.x = 0.0f;
	velocity.y = 0.0f;
	projectileSpeed = 10.0f;

	// check for direction determined in samus class
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
	
	body->SetLinearVelocity(velocity);
	// alter position each frame
	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);

	m_CurrentSheetlessAnimation->update(deltaTime);
}

void DefaultBullet::draw(Renderer& renderer)
{
	renderer.draw(m_CurrentSheetlessAnimation->getCurrentFrame(), position, sf::Vector2f(0.3f, 0.3f));
}

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

Missile::~Missile()
{
	if (body)
	{
		// Destroy the fixture
		body->DestroyFixture(missileFixture);

		// Destroy the body
		Physics::world.DestroyBody(body);

		body = nullptr;  // Set the pointer to nullptr to avoid using it accidentally later
	}
}

void Missile::begin(b2Vec2 initialPosition)
{
	createFixture(initialPosition);
	projectileSpeed = 0.5f;
}

void Missile::update(float deltaTime)
{
	m_MaxSpeed = 15.0f;

	if (projectileSpeed <= m_MaxSpeed)
	{
		projectileSpeed *= 2.0f;
	}

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

	body->SetLinearVelocity(velocity);
	// alter position each frame
	position = sf::Vector2f(body->GetPosition().x, body->GetPosition().y);
}

void Missile::draw(Renderer& renderer)
{
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
}
