// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Tower.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Bullet.h"
#include "AIState.h"

Tower::Tower(class Game* game)
:Actor(game)
{
	SpriteComponent* sc = new SpriteComponent(this, 200);
	sc->SetTexture(game->GetTexture("Assets/Tower.png"));
	
	mMove = new MoveComponent(this);
	//mMove->SetAngularSpeed(Math::Pi);
	
	mNextAttack = AttackTime;

	// AIComponent���쐬
	mAic = new AIComponent(this);

	// ��Ԃ�AIComponent�ɓo�^
	mAic->RegisterState(new TowerPatrol(mAic, this));
	mAic->RegisterState(new TowerDeath(mAic, this));
	mAic->RegisterState(new TowerAttack(mAic, this));

	// Patrol�ɏ�����
	mAic->ChangeState("Patrol");
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	


	/*
	if (GetGame()->GetNearestEnemy(GetPosition()) != nullptr)
	{
		// ��ԋ߂��G��ۑ�
		mEnemy = GetGame()->GetNearestEnemy(GetPosition());
		
		if (mAic->GetCurrentState()->GetName() == "Attack")
		{
			// Attack�ɐ؂�ւ�
			mAic->ChangeState("Attack");
		}

		// ���L���X�g�^�C�����v�Z
		mNextAttack += AttackTime;
	}
	else
	{
		mAic->ChangeState("Patrol");
	}

	
	if (mNextAttack <= 0.0f)
	{
		// ��ԋ߂��G���擾
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
		
		// �G�����݂�����
		if (e != nullptr)
		{
			// ��������G�ɑ΂���x�N�g�����v�Z
			Vector2 dir = e->GetPosition() - GetPosition();
			// �G�Ƃ̋������v�Z
			float dist = dir.Length();
			// �U���͈͂ɓG��������
			if (dist < AttackRange)
			{
				// �G�̕����ɕ����]��
				SetRotation(Math::Atan2(-dir.y, dir.x));

				// ��(����)�̍��W�Ƀo���b�g�𐶐�
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		
	}
	*/
}
