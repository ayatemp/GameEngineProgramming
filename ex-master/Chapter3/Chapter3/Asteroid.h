#pragma once
#include "Actor.h"
#include "MoveComponent.h"

class Asteroid : public Actor
{
public:
	Asteroid(class Game* game);
	~Asteroid();
	/*-------------------------追加した箇所-------------------------------------*/

	void UpdateActor(float deltaTime);
	/*-------------------------------ここまで-----------------------------------*/

	class CircleComponent* GetCircle() { return mCircle; }

private:
	class CircleComponent* mCircle;

	/*-------------------------追加した箇所-------------------------------------*/

	class MoveComponent* mMove;

	/*-------------------------------ここまで-----------------------------------*/

};