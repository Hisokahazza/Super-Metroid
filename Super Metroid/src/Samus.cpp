#include "Samus.h"

#include <iostream>

void Samus::setAnimationState(AnimationState state)
{
	m_CurrentAnimationState = state;
}

// Assign current direction every frame so that each bullet object can be assigned its own direction
Direction Samus::currentDirection()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
	{
		return UPLEFT;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
	{
		return UPRIGHT;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		return DOWNLEFT;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D) && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		return DOWNRIGHT;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::W))
	{
		return UP;
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::S))
	{
		return DOWN;
	}

	else if (m_Orientation == RIGHT)
	{
		return RIGHT;
	}

	else if (m_Orientation == LEFT)
	{
		return LEFT;
	}
}

void Samus::updateBullet(float deltaTime)
{
	for (auto bullet : m_Bullets)
	{
		if (&(bullet->fixtureData) == sporeSpawn.getProjectileDestroyed())
		{
			bullet->destroyed = true;
		}

		if (bullet->destroyed == true)
		{
			m_Bullets.erase(std::find(m_Bullets.begin(), m_Bullets.end(), bullet));
			bullet->~DefaultBullet();
			delete(bullet);
		}
		else if (bullet)
		{
			bullet->update(deltaTime);
		}
	}
}

void Samus::updateMissile(float deltaTime)
{
	for (auto missile : m_Missiles)
	{
		if ( missile && &(missile->fixtureData) == sporeSpawn.getProjectileDestroyed())
		{
			missile->destroyed = true;
		}

		if (missile->destroyed == true)
		{
			m_Missiles.erase(std::find(m_Missiles.begin(), m_Missiles.end(), missile));
			missile->~Missile();
			delete(missile);
		}
		else if (missile)
		{
			missile->update(deltaTime);
		}
	}
}

void Samus::createFixture()
{	
	fixtureData.listener = this;
	fixtureData.type = SAMUS;
	fixtureData.samus = this;
	
	// Create character body
	b2BodyDef bodyDef{};
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(position.x, position.y);
	bodyDef.fixedRotation = true;
	m_Body = Physics::world.CreateBody(&bodyDef);

	// Create capsule fixture for character collisions (this ensures smooth passing along the ground)
	b2PolygonShape polygonShape{};
	polygonShape.SetAsBox(0.55f, 0.5f);
	b2FixtureDef fixtureDef{};
	fixtureDef.userData.pointer = (uintptr_t)&fixtureData;
	fixtureDef.shape = &polygonShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.0f;
	fixtureDef.restitution = 0.0f;
	m_Body->CreateFixture(&fixtureDef);

	polygonShape.SetAsBox(0.5f, 0.3f, b2Vec2(0.0f, 1.2f), 0.0f);
	fixtureDef.isSensor = true;
	samusGroundFixture = m_Body->CreateFixture(&fixtureDef);

	b2CircleShape circleShape{};
	circleShape.m_radius = 0.55f;
	circleShape.m_p.Set(0.0f, -0.65f);
	fixtureDef.shape = &circleShape;
	fixtureDef.isSensor = false;
	m_Body->CreateFixture(&fixtureDef);
	circleShape.m_p.Set(0.0f, 0.65f);
	m_Body->CreateFixture(&fixtureDef);

	// Generate hitboxes
	fixtureDef.isSensor = true;
	polygonShape.SetAsBox(0.4f, 0.9f);
	fixtureDef.shape = &polygonShape;
	samusHitbox = m_Body->CreateFixture(&fixtureDef);
	
	polygonShape.SetAsBox(0.4f, 0.55f, b2Vec2(0.0f, 0.5f), 0.0f);
	crouchHitbox = m_Body->CreateFixture(&fixtureDef);

	polygonShape.SetAsBox(0.4f, 0.3f, b2Vec2(0.0f, 0.9f), 0.0f);
	morphballHitbox = m_Body->CreateFixture(&fixtureDef);

	polygonShape.SetAsBox(0.4f, 0.6f, b2Vec2(0.0f, 0.1f), 0.0f);
	jumpHitbox = m_Body->CreateFixture(&fixtureDef);
}

void Samus::createActiveAnimations()
{
	// Define animation states
	m_Animations[IDLE] = new Animation(Resources::textures["Idle_Samus.png"], "Idle_Samus.png", sf::Vector2u(1, 1), 1);

	m_Animations[RUNRIGHT] = new Animation(Resources::textures["Run_Right.png"], "Run_Right.png", sf::Vector2u(10, 1), 10);
	m_Animations[RUNLEFT] = new Animation(Resources::textures["Run_Left.png"], "Run_Left.png", sf::Vector2u(10, 1), 10);

	m_Animations[JUMPRIGHT] = new Animation(Resources::textures["Jump_Right.png"], "Jump_Right.png", sf::Vector2u(9, 1), 9);
	m_Animations[JUMPLEFT] = new Animation(Resources::textures["Jump_Left.png"], "Jump_Left.png", sf::Vector2u(9, 1), 9);
	m_Animations[JUMPRIGHTSTATIC] = new Animation(Resources::textures["Jump_Static_Right.png"], "Jump_Static_Right.png", sf::Vector2u(2, 1), 2, false, 1.0f);
	m_Animations[JUMPLEFTSTATIC] = new Animation(Resources::textures["Jump_Static_Left.png"], "Jump_Static_Left.png", sf::Vector2u(2, 1), 2, false, 1.0f);

	m_Animations[SHOOTRIGHT] = new Animation(Resources::textures["Shoot_Static_Right.png"], "Shoot_Static_Right.png", sf::Vector2u(4, 1), 4);
	m_Animations[SHOOTLEFT] = new Animation(Resources::textures["Shoot_Static_Left.png"], "Shoot_Static_Left.png", sf::Vector2u(4, 1), 4);
	m_Animations[SHOOTUPORIENTATIONRIGHTSTATIC] = new Animation(Resources::textures["Shoot_Static_Up_Orientation_Right.png"], "Shoot_Static_Up_Orientation_Right.png", sf::Vector2u(2, 1), 2, false, 0.1, 1, false);
	m_Animations[SHOOTUPORIENTATIONLEFTSTATIC] = new Animation(Resources::textures["Shoot_Static_Up_Orientation_Left.png"], "Shoot_Static_Up_Orientation_Left.png", sf::Vector2u(2, 1), 2, false, 0.1, 1, false);
	m_Animations[SHOOTRIGHTSTATIC] = new Animation(Resources::textures["Shoot_Static_Right.png"], "Shoot_Static_Right.png", sf::Vector2u(4, 1), 4, true);
	m_Animations[SHOOTLEFTSTATIC] = new Animation(Resources::textures["Shoot_Static_Left.png"], "Shoot_Static_Left.png", sf::Vector2u(4, 1), 4, true);
	m_Animations[SHOOTRIGHTMOVING] = new Animation(Resources::textures["Shoot_Moving_Right.png"], "Shoot_Moving_Right.png", sf::Vector2u(10, 1), 10, false, 0.07f);
	m_Animations[SHOOTLEFTMOVING] = new Animation(Resources::textures["Shoot_Moving_Left.png"], "Shoot_Moving_Left.png", sf::Vector2u(10, 1), 10, false, 0.07f);
	m_Animations[SHOOTUPRIGHTSTATIC] = new Animation(Resources::textures["Shoot_Static_Up_Right.png"], "Shoot_Static_Up_Right.png", sf::Vector2u(1, 1));
	m_Animations[SHOOTUPLEFTSTATIC] = new Animation(Resources::textures["Shoot_Static_Up_Left.png"], "Shoot_Static_Up_Left.png", sf::Vector2u(1, 1));
	m_Animations[SHOOTDOWNRIGHTSTATIC] = new Animation(Resources::textures["Shoot_Static_Down_Right.png"], "Shoot_Static_Down_Right.png", sf::Vector2u(1, 1));
	m_Animations[SHOOTDOWNLEFTSTATIC] = new Animation(Resources::textures["Shoot_Static_Down_Left.png"], "Shoot_Static_Down_Left.png", sf::Vector2u(1, 1));
	m_Animations[SHOOTUPRIGHTMOVING] = new Animation(Resources::textures["Shoot_Moving_Up_Right.png"], "Shoot_Moving_Up_Right.png", sf::Vector2u(10, 1), 10);
	m_Animations[SHOOTUPLEFTMOVING] = new Animation(Resources::textures["Shoot_Moving_Up_Left.png"], "Shoot_Moving_Up_Left.png", sf::Vector2u(10, 1), 10);
	m_Animations[SHOOTDOWNRIGHTMOVING] = new Animation(Resources::textures["Shoot_Moving_Down_Right.png"], "Shoot_Moving_Down_Right.png", sf::Vector2u(10, 1), 10);
	m_Animations[SHOOTDOWNLEFTMOVING] = new Animation(Resources::textures["Shoot_Moving_Down_Left.png"], "Shoot_Moving_Down_Left.png", sf::Vector2u(10, 1), 10);

	m_Animations[CROUCHRIGHT] = new Animation(Resources::textures["Crouch_Right.png"], "Crouch_Right.png", sf::Vector2u(4, 1), 4, true);
	m_Animations[CROUCHLEFT] = new Animation(Resources::textures["Crouch_Left.png"], "Crouch_Left.png", sf::Vector2u(4, 1), 4, true);
	m_Animations[MORPHBALLRIGHT] = new Animation(Resources::textures["Morph_Ball_Right.png"], "Morph_Ball_Right.png", sf::Vector2u(8, 1), 8);
	m_Animations[MORPHBALLLEFT] = new Animation(Resources::textures["Morph_Ball_Left.png"], "Morph_Ball_Left.png", sf::Vector2u(8, 1), 8);
}

Samus::Samus() : m_NumGroundContacts(0), m_JumpDelayCount(0)
{
}

// Call functions relating to samus character initialisation
void Samus::begin()
{
	createFixture();

	createActiveAnimations();

	// Begin all active animations
	for (auto state : m_ActiveStates)
	{
		m_Animations[state]->begin();
	}

	m_ActiveProjectile = BULLETPROJ;
}

// Update every frame (passed into game update function)
void Samus::update(float deltaTime)
{
	float move = movementSpeed;
	m_CanShoot = true;
	currentHitbox = samusHitbox;

	// Determine relevant idle animation state
	if (m_Orientation == RIGHT)
	{
		setAnimationState(SHOOTRIGHTSTATIC);

		m_ProjectileInitialPos = b2Vec2(position.x + 0.5f, position.y);
	}
	else if (m_Orientation == LEFT)
	{
		setAnimationState(SHOOTLEFTSTATIC);
		m_ProjectileInitialPos = b2Vec2(position.x - 0.5f, position.y);
	}

	if (m_Orientation == RIGHT && m_NumGroundContacts < 1)
	{
		setAnimationState(JUMPRIGHTSTATIC);
	}
	else if (m_Orientation == LEFT && m_NumGroundContacts < 1)
	{
		setAnimationState(JUMPLEFTSTATIC);
	}

	// Get current velocity of body
	b2Vec2 velocity = m_Body->GetLinearVelocity();
	velocity.x = 0.0f;

	// Handle sprint
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::LShift) && m_NumGroundContacts >= 1)
	{
		m_CanShoot = false;
		move *= 2;
		m_Sprint = true;
	}

	// Handle left right movement
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Right))
	{
		if (m_CrouchState == CROUCH)
		{
			m_CrouchState = NONE;
		}

		if (m_NumGroundContacts >= 1)
		{
			velocity.x += move;
		}
		// movement is restricted when in the air
		else
		{
			velocity.x += move / 1.3;
		}

		if (m_NumGroundContacts < 1)
		{
			setAnimationState(JUMPRIGHT);
		}
		else if (m_Sprint == true)
		{
			setAnimationState(RUNRIGHT);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			setAnimationState(SHOOTUPRIGHTMOVING);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			setAnimationState(SHOOTDOWNRIGHTMOVING);
		}
		else
		{
			setAnimationState(SHOOTRIGHTMOVING);
			m_ProjectileInitialPos = b2Vec2(position.x + 1.0f, position.y - 0.1f);
		}

		// Set orientation based on players last movement
		m_Orientation = RIGHT;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Left))
	{
		if (m_CrouchState == CROUCH)
		{
			m_CrouchState = NONE;
		}

		if (m_NumGroundContacts >= 1)
		{
			velocity.x -= move;
		}
		// movement is restricted when in the air
		else
		{
			velocity.x -= move / 1.3;
		}

		if (m_NumGroundContacts < 1)
		{
			setAnimationState(JUMPLEFT);
		}
		else if (m_Sprint == true)
		{
			setAnimationState(RUNLEFT);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			setAnimationState(SHOOTUPLEFTMOVING);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			setAnimationState(SHOOTDOWNLEFTMOVING);
		}
		else
		{
			setAnimationState(SHOOTLEFTMOVING);
			m_ProjectileInitialPos = b2Vec2(position.x - 0.5f, position.y - 0.1f);
		}

		// Set orientation based on players last movement
		m_Orientation = LEFT;
	}


	// Handle jumping
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) && m_NumGroundContacts >= 1 && m_CrouchState != MORPHBALL)
	{
		velocity.y = -jumpVelocity;
	}

	// allow player to jump higher when holding the key
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) && m_NumGroundContacts < 1)
	{
		float downwardForce = 100.0f;
		// Apply an initial force vertically downwards when key is released
		m_Body->ApplyForceToCenter(b2Vec2(0, downwardForce), true);

		m_JumpDelayCount++;

		// After delay apply a smaller force vertically upward to minimise acceleration when falling
		if (m_JumpDelayCount > 2)
		{
			m_Body->ApplyForceToCenter(b2Vec2(0, -downwardForce / 2.5), true);
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_NumGroundContacts >= 1)
	{
		if (m_CrouchState == NONE)
		{
			m_CrouchState = CROUCH;
		}
		else if (m_CrouchState == CROUCH && m_ReleasedCrouch == true)
		{
			m_CrouchState = MORPHBALL;
		}
		else if (m_CrouchState == MORPHBALL && m_ReleasedCrouch == true)
		{
			m_ReleasedCrouch = false;
		}
	}

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && m_CrouchState != NONE)
	{
		m_ReleasedCrouch = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Up) && m_NumGroundContacts >= 1)
	{
		if (m_CrouchState == CROUCH && m_ReleasedCrouch == true)
		{
			m_CrouchState = NONE;
			m_ReleasedCrouch = false;
		}
		else if (m_CrouchState == MORPHBALL && m_ReleasedCrouch == true)
		{
			m_ReleasedCrouch = false;
			m_CrouchState = CROUCH;
		}
	}

	if (m_CrouchState == CROUCH && (!sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)))
	{
		currentHitbox = crouchHitbox;

		if (m_Orientation == RIGHT)
		{
			setAnimationState(CROUCHRIGHT);
		}
		else if (m_Orientation == LEFT)
		{
			setAnimationState(CROUCHLEFT);
		}
	}

	if (m_CrouchState == MORPHBALL)
	{
		m_CanShoot = false;

		currentHitbox = morphballHitbox;

		if (m_Orientation == RIGHT)
		{
			setAnimationState(MORPHBALLRIGHT);
		}
		else if (m_Orientation == LEFT)
		{
			setAnimationState(MORPHBALLLEFT);
		}
	}

	// Handle aiming animation
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m_Orientation == RIGHT)
	{
		setAnimationState(SHOOTUPORIENTATIONRIGHTSTATIC);
		m_ProjectileInitialPos = b2Vec2(position.x + 0.1f, position.y - 0.8f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && m_Orientation == LEFT)
	{
		setAnimationState(SHOOTUPORIENTATIONLEFTSTATIC);
		m_ProjectileInitialPos = b2Vec2(position.x, position.y - 0.8f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::D) && m_Orientation == RIGHT && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		setAnimationState(SHOOTUPRIGHTSTATIC);
		m_ProjectileInitialPos = b2Vec2(position.x + 0.3f, position.y - 0.4f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::A) && m_Orientation == LEFT && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		setAnimationState(SHOOTUPLEFTSTATIC);
		m_ProjectileInitialPos = b2Vec2(position.x - 0.3f, position.y - 0.4f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D) && m_Orientation == RIGHT && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		setAnimationState(SHOOTDOWNRIGHTSTATIC);
		m_ProjectileInitialPos = b2Vec2(position.x + 0.3f, position.y);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::A) && m_Orientation == LEFT && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		setAnimationState(SHOOTDOWNLEFTSTATIC);
		m_ProjectileInitialPos = b2Vec2(position.x - 0.3f, position.y);
	}

	// Handle weapon swapping
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
	{
		m_ActiveProjectile = BULLETPROJ;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
	{
		m_ActiveProjectile = MISSILEPROJ;
	}

	// Handle shooting
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::F) && m_CanShoot == true)
	{
		if (m_ActiveProjectile == BULLETPROJ)
		{
			// Delay between shots
			if (m_ProjectileDelayCount >= m_ProjectileDelay)
			{
				m_ProjectileDelayCount = 0;

				// Push new bullet object to array so that multiple existing bullets can be handled
				DefaultBullet* bullet = new DefaultBullet(currentDirection());
				m_Bullets.push_back(bullet);
				m_Bullets.back()->begin(m_ProjectileInitialPos);

				m_ProjectileShot = true;
			}
		}
		else if (m_ActiveProjectile == MISSILEPROJ)
		{
			if (m_ProjectileDelayCount >= m_ProjectileDelay && m_MissilesUsed < 15)
			{
				m_ProjectileDelayCount = 0;

				// Push new bullet object to array so that multiple existing bullets can be handled
				Missile* missile = new Missile(currentDirection());
				m_Missiles.push_back(missile);
				m_Missiles.back()->begin(m_ProjectileInitialPos);

				m_ProjectileShot = true;
				m_MissileOffset--;
				m_MissilesUsed++;
			}
		}

		// Handle idle animation
		if (m_Orientation == RIGHT && m_CurrentAnimationState == SHOOTRIGHTSTATIC)
		{
			setAnimationState(SHOOTRIGHT);
		}
		else if (m_Orientation == LEFT && m_CurrentAnimationState == SHOOTLEFTSTATIC)
		{
			setAnimationState(SHOOTLEFT);
		}

		// Increments delay count
		m_ProjectileDelayCount++;
	}

	// Checks if a bullet has been shot or destroyed and updates all bullets
	if (m_ProjectileShot)
	{
		if (m_ActiveProjectile == BULLETPROJ)
		{
			if (m_Missiles.size() != 0)
			{
				updateMissile(deltaTime);
			}
			else
			{
				updateBullet(deltaTime);
			}
		}
		else if (m_ActiveProjectile == MISSILEPROJ)
		{
			if (m_Bullets.size() != 0)
			{
				updateBullet(deltaTime);
			}
			else
			{
				updateMissile(deltaTime);
			}
		}
	}

	// Set jump hitbox
	if (m_CurrentAnimationState == JUMPRIGHT || m_CurrentAnimationState == JUMPLEFT)
	{
		currentHitbox = jumpHitbox;
	}

	// handle samus getting hit by bosses
	if (m_SamusHit == true)
	{
		velocity = { 0, 0 };

		b2Vec2 knockBackImpluse;
		float m_KnockBackSpeed = 10.0f;
		if (m_Orientation == RIGHT)
		{
			knockBackImpluse = b2Vec2(-m_KnockBackSpeed, -m_KnockBackSpeed / 10);
		}
		if (m_Orientation == LEFT)
		{
			knockBackImpluse = b2Vec2(m_KnockBackSpeed, -m_KnockBackSpeed / 10);
		}

		m_Body->ApplyForceToCenter(knockBackImpluse, true);
		m_SamusHit = false;
	}

	m_Animations[m_CurrentAnimationState]->update(deltaTime);

	// Set the current samus hitbox from bosses
	sporeSpawn.setPlayerHitbox(currentHitbox);

	// retrieve health offset from boss
	m_CurrentHealthOffset = sporeSpawn.getPlayerHealthOffset();

	// Update HUD elements
	playerHUD.update(deltaTime, m_CurrentHealthOffset, m_MissileOffset, m_ActiveProjectile);
	
	m_MissileOffset = 0;
	sporeSpawn.setPlayerHealthOffset(0);
	
	m_Sprint = false;

	// Set velocity to new value
	m_Body->SetLinearVelocity(velocity);

	// Assign new character position
	position = sf::Vector2f(m_Body->GetPosition().x, m_Body->GetPosition().y);
}

// handle render calls
void Samus::draw(Renderer& renderer)
{
	renderer.draw(m_Animations[m_CurrentAnimationState]->getCurrentFrame(), position,
		sf::Vector2f(4.0f, 3.7f));

	if (m_Bullets.size() != 0)
	{
		for (auto bullet : m_Bullets)
		{
			bullet->draw(renderer);
		}
	}
	if (m_Missiles.size() != 0)
	{
		for (auto missile : m_Missiles)
		{
			missile->draw(renderer);
		}
	}
		
}

// Implement collision listener in physics module
void Samus::onBeginContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* otherData = (FixtureData*)other->GetUserData().pointer;
	FixtureData* selfData = (FixtureData*)self->GetUserData().pointer;

	if (!otherData)
	{
		return;
	}

	if (samusGroundFixture == self && otherData->type == MAPTILE)
	{
		m_NumGroundContacts++;
	}
	if (self == currentHitbox && (otherData->type == BOSSCOMPONENT || otherData->type == BOSS))
	{
		std::cout << "COLLIDED" << std::endl;
		m_SamusHit = true;
	}
}
void Samus::onEndContact(b2Fixture* self, b2Fixture* other)
{
	FixtureData* data = (FixtureData*)other->GetUserData().pointer;

	if (!data)
	{
		return;
	}

	if (samusGroundFixture == self && data->type == MAPTILE && m_NumGroundContacts > 0)
	{
		m_NumGroundContacts--;
	}
}
