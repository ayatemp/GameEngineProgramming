// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AIState.h"
#include "AIComponent.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Grid.h"
#include "Tile.h"
#include <SDL_log.h>

/*--------------------------------------------------*/
// TowerPatrol 
void TowerPatrol::Update(float deltaTime)
{
	//SDL_Log("Updating %s state", GetName());
	
	if (mTower->GetGame()->GetNearestEnemy(mTower->GetPosition()) != nullptr)
	{
		// ��ԋ߂��G��ۑ�
		mEnemy = mTower->GetGame()->GetNearestEnemy(mTower->GetPosition());
		

		if (mOwner->GetCurrentState()->GetName() != "Attack")
		{
			// Attack�ɐ؂�ւ�
			mOwner->ChangeState("Attack");
		}

	}
	
}

void TowerPatrol::OnEnter()
{
	SDL_Log("Tower Entering %s state", GetName());
}

void TowerPatrol::OnExit()
{
	SDL_Log("Tower Exiting %s state", GetName());
}
/*--------------------------------------------------*/


/*--------------------------------------------------*/
// TowerDeath
void TowerDeath::Update(float deltaTime)
{
	SDL_Log("Tower Updating %s state", GetName());
}

void TowerDeath::OnEnter()
{
	SDL_Log("Tower Entering %s state", GetName());
}

void TowerDeath::OnExit()
{
	SDL_Log("Tower Exiting %s state", GetName());
}
/*--------------------------------------------------*/


/*--------------------------------------------------*/
// TowerAttack
void TowerAttack::Update(float deltaTime)
{
	mTower->SetNextAttack(mTower->GetNextAttack() - deltaTime);

	if (mTower->GetNextAttack() <= 0.0f)
	{
		mEnemy = mTower->GetGame()->GetNearestEnemy(mTower->GetPosition());
		if (mEnemy != nullptr)
		{
			// ��������G�ɑ΂���x�N�g�����v�Z
			Vector2 dir = mEnemy->GetPosition() - mTower->GetPosition();
			// �G�Ƃ̋������v�Z
			float dist = dir.Length();
			// �U���͈͂ɓG��������
			if (dist < mTower->GetAttackRange())
			{
				// �G�̕����ɕ����]��
				mTower->SetRotation(Math::Atan2(-dir.y, dir.x));

				// ��(����)�̍��W�Ƀo���b�g�𐶐�
				Bullet* b = new Bullet(mTower->GetGame());
				b->SetPosition(mTower->GetPosition());
				b->SetRotation(mTower->GetRotation());
			}
			PatrolTime = PATROL_TIME;

		}
		// ���L���X�g�^�C�����v�Z
		mTower->SetNextAttack(mTower->GetNextAttack() + mTower->GetAttackTime());

	}
	PatrolTime -= deltaTime;

	if (PatrolTime <= 0)
	{
		mOwner->ChangeState("Patrol");
		PatrolTime = PATROL_TIME;
	}

	//SDL_Log("Updating %s state", GetName());
}

void TowerAttack::OnEnter()
{
	SDL_Log("Tower Entering %s state", GetName());
}

void TowerAttack::OnExit()
{
	SDL_Log("Tower Exiting %s state", GetName());
}
/*--------------------------------------------------*/



/*--------------------------------------------------*/
// EnemyPatrol 
void EnemyPatrol::Update(float deltaTime)
{
	//SDL_Log("Updating %s state", GetName());

	// Am I near the end tile ?
	Vector2 diff = mEnemy->GetPosition() - mEnemy->GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if (Math::NearZero(diff.Length(), 10.0f))
	{
		mOwner->ChangeState("Death");
	}

}

void EnemyPatrol::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void EnemyPatrol::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}
/*--------------------------------------------------*/


/*--------------------------------------------------*/
// EnemyDeath
void EnemyDeath::Update(float deltaTime)
{
	mEnemy->SetState(mEnemy->EDead);
	SDL_Log("Updating %s state", GetName());
}

void EnemyDeath::OnEnter()
{
	SDL_Log("Entering %s state", GetName());
}

void EnemyDeath::OnExit()
{
	SDL_Log("Exiting %s state", GetName());
}
/*--------------------------------------------------*/