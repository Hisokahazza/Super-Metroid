#include "Physics.h"
#include "iostream"

b2World Physics::world{ b2Vec2(0.0f, 9.81f) };
DebugDrawImp* Physics::debugDrawObj{};


class DebugDrawImp : public b2Draw
{
private:
	sf::RenderTarget& m_Target;

public:

	DebugDrawImp(sf::RenderTarget& target) :
		m_Target(target)
	{
	}

	// Inherited via b2Draw
	// Implement virtual functions related to debug drawing
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		sf::ConvexShape shape(vertexCount);

		for (size_t count = 0; count < vertexCount; count++)
		{
			shape.setPoint(count, sf::Vector2f(vertices[count].x, vertices[count].y));
		}

		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		shape.setOutlineThickness(0.02f);

		m_Target.draw(shape);
	}

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
	{
		sf::ConvexShape shape(vertexCount);

		for (size_t count = 0; count < vertexCount; count++)
		{
			shape.setPoint(count, sf::Vector2f(vertices[count].x, vertices[count].y));
		}

		shape.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 80));

		m_Target.draw(shape);
	}

	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override
	{
		sf::CircleShape circle(radius);
		circle.setPosition(center.x, center.y);
		circle.setOrigin(radius, radius);
		circle.setFillColor(sf::Color::Transparent);
		circle.setOutlineColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		circle.setOutlineThickness(0.02f);
		m_Target.draw(circle);

	}

	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override
	{
		sf::CircleShape circle(radius);
		circle.setPosition(center.x, center.y);
		circle.setOrigin(radius, radius);
		circle.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 80));
		m_Target.draw(circle);

		b2Vec2 p = center + (radius * axis);
		DrawSegment(center, p, color);
	}

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
	{
		sf::VertexArray va(sf::Lines, 2);
		sf::Color sfColour = sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
		va[0].position = sf::Vector2f(p1.x, p1.y);
		va[0].color = sfColour;

		va[1].position = sf::Vector2f(p2.x, p2.y);
		va[1].color = sfColour;

		m_Target.draw(va);
	}

	void DrawTransform(const b2Transform& xf) override
	{
		b2Vec2 p = xf.p;
		b2Vec2 px = p + (0.5f * xf.q.GetXAxis());
		b2Vec2 py = p + (0.5f * xf.q.GetYAxis());

		DrawSegment(p, px, b2Color(1.0f, 1.0f, 0.0f));
		DrawSegment(p, py, b2Color(1.0f, 1.0f, 0.0f));

	}

	void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override
	{
		sf::CircleShape circle(size);
		circle.setPosition(p.x, p.y);
		circle.setOrigin(size, size);
		circle.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255, color.a * 255));
		m_Target.draw(circle);
	}

};


// implement listener
class CollisionListenerGlobal : public b2ContactListener
{
	virtual void BeginContact(b2Contact* contact) override
	{
		FixtureData* dataA = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;
		FixtureData* dataB = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;

		if (dataA && dataA->listener && dataA->isActive == true)
		{
			dataA->listener->onBeginContact(contact->GetFixtureA(), contact->GetFixtureB());
		}

		if (dataB && dataB->listener && dataB->isActive == true)
		{
			dataB->listener->onBeginContact(contact->GetFixtureB(), contact->GetFixtureA());
		}
	}

	virtual void EndContact(b2Contact* contact) override
	{
		FixtureData* dataA = (FixtureData*)contact->GetFixtureA()->GetUserData().pointer;
		FixtureData* dataB = (FixtureData*)contact->GetFixtureB()->GetUserData().pointer;

		if (dataA && dataA->listener && dataA->isActive == true)
		{
			dataA->listener->onEndContact(contact->GetFixtureA(), contact->GetFixtureB());
		}

		if (dataB && dataB->listener && dataB->isActive == true)
		{
			dataB->listener->onEndContact(contact->GetFixtureB(), contact->GetFixtureA());
		}
	}
};

void Physics::init()
{

}

void Physics::update(float deltaTime)
{
	world.Step(deltaTime, 6, 2);
	// Sets world listener
	world.SetContactListener(new CollisionListenerGlobal());
}

void Physics::debugDraw(Renderer& renderer)
{
	if (!debugDrawObj)
	{
		// Initialise debugDraw object 
		debugDrawObj = new DebugDrawImp(renderer.target);
		// Sets flags for debug drawer to pick up
		//debugDrawObj->SetFlags(b2Draw::e_shapeBit);
		// Sets the world debug drawer
		world.SetDebugDraw(debugDrawObj);
	}

	world.DebugDraw();
}


