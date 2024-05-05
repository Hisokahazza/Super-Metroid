#pragma once

#include "Physics.h"
#include "Renderer.h"

#include "box2d/b2_polygon_shape.h"

class Door : public Collisionlistener
{
private:
	void createFixture();
public:
	void Begin();
	void Update(float deltaTime);
	void Draw(Renderer& renderer);
};