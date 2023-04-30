#pragma once
#include "Actor.h"
#include "MoveComponent.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();
	/*-------------------------�ǉ������ӏ�-------------------------------------*/

	void UpdateActor(float deltaTime);
	/*-------------------------------�����܂�-----------------------------------*/

	class CircleComponent* GetCircle() { return mCircle; }

private:
	class CircleComponent* mCircle;

	/*-------------------------�ǉ������ӏ�-------------------------------------*/

	class MoveComponent* mMove;

	/*-------------------------------�����܂�-----------------------------------*/

};