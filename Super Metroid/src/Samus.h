#pragma once

#pragma once

#include <unordered_map>
#include <Queue>

#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_circle_shape.h>
#include <box2d/b2_fixture.h>

#include "Physics.h"
#include "Renderer.h"
#include "Resources.h"

#include "Animation.h"
#include "Projectile.h"

#include "HUD.h"
#include "Boss.h"

extern HUD playerHUD;
extern SporeSpawn sporeSpawn;

enum AnimationState
{
	IDLE,

	RUNRIGHT,
	RUNLEFT,

	JUMPRIGHT,
	JUMPLEFT,
	JUMPRIGHTSTATIC,
	JUMPLEFTSTATIC,

	SHOOTRIGHT,
	SHOOTLEFT,
	SHOOTUPORIENTATIONRIGHTSTATIC,
	SHOOTUPORIENTATIONLEFTSTATIC,
	SHOOTRIGHTSTATIC,
	SHOOTLEFTSTATIC,
	SHOOTRIGHTMOVING,
	SHOOTLEFTMOVING,
	SHOOTUPRIGHTSTATIC,
	SHOOTUPLEFTSTATIC,
	SHOOTDOWNRIGHTSTATIC,
	SHOOTDOWNLEFTSTATIC,
	SHOOTUPRIGHTMOVING,
	SHOOTUPLEFTMOVING,
	SHOOTDOWNRIGHTMOVING,
	SHOOTDOWNLEFTMOVING,

	CROUCHRIGHT,
	CROUCHLEFT,
	MORPHBALLRIGHT,
	MORPHBALLLEFT
};

enum CrouchState
{
	NONE,
	CROUCH,
	MORPHBALL
};

class Samus : public Collisionlistener
{
private:
	void createFixture();
	void createActiveAnimations();

	void updateBullet(float deltaTime);
	void updateMissile(float deltaTime);

	std::vector<DefaultBullet*> m_Bullets;
	std::vector<Missile*> m_Missiles;

	b2Body* m_Body;

	int m_JumpDelayCount = 0;
	int m_ProjectileDelayCount = 20;
	int m_ProjectileDelay = 20;

	int m_NumGroundContacts = 0;

	bool m_ProjectileShot = false;
	bool m_CanShoot = true;
	ProjectileType m_ActiveProjectile;
	b2Vec2 m_ProjectileInitialPos;
	int m_MissileOffset;
	int m_MissilesUsed;

	std::unordered_map<AnimationState, Animation*> m_Animations;
	std::vector<AnimationState> m_ActiveStates = { IDLE,
		RUNRIGHT,
		RUNLEFT,
		JUMPRIGHT,
		JUMPLEFT,
		JUMPRIGHTSTATIC,
		JUMPLEFTSTATIC,
		SHOOTRIGHTSTATIC,
		SHOOTLEFTSTATIC,
		SHOOTRIGHTMOVING,
		SHOOTLEFTMOVING,
		SHOOTUPRIGHTSTATIC,
		SHOOTUPLEFTSTATIC,
		SHOOTDOWNRIGHTSTATIC,
		SHOOTDOWNLEFTSTATIC,
		SHOOTUPRIGHTMOVING,
		SHOOTUPLEFTMOVING,
		SHOOTDOWNRIGHTMOVING,
		SHOOTDOWNLEFTMOVING,
		SHOOTRIGHT,
		SHOOTLEFT,
		SHOOTUPORIENTATIONRIGHTSTATIC,
		SHOOTUPORIENTATIONLEFTSTATIC,
		CROUCHRIGHT,
		CROUCHLEFT,
		MORPHBALLRIGHT,
		MORPHBALLLEFT
	};
	AnimationState m_CurrentAnimationState;
	void setAnimationState(AnimationState animationState);

	bool m_Sprint = false;

	CrouchState m_CrouchState = NONE;
	bool m_ReleasedCrouch = false;

	Direction m_Orientation;
	Direction currentDirection();

	int m_CurrentHealthOffset;

	bool m_SamusHit = false;
public:
	b2Fixture* currentHitbox;

	const float movementSpeed = 7.0f;
	const float jumpVelocity = 11.0f;

	sf::Texture spriteSheet;

	Samus();

	void begin();
	void update(float deltaTime);
	void draw(Renderer& renderer);

	sf::Vector2f position{};

	b2Fixture* samusGroundFixture;
	b2Fixture* samusHitbox;
	b2Fixture* crouchHitbox;
	b2Fixture* morphballHitbox;
	b2Fixture* jumpHitbox;

	FixtureData fixtureData{};

	// Inherited via Collisionlistener
	void onBeginContact(b2Fixture* self, b2Fixture* other) override;
	void onEndContact(b2Fixture* self, b2Fixture* other) override;
};