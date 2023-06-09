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

	// AIComponentを作成
	mAic = new AIComponent(this);

	// 状態をAIComponentに登録
	mAic->RegisterState(new TowerPatrol(mAic, this));
	mAic->RegisterState(new TowerDeath(mAic, this));
	mAic->RegisterState(new TowerAttack(mAic, this));

	// Patrolに初期化
	mAic->ChangeState("Patrol");
}

void Tower::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);
	


	/*
	if (GetGame()->GetNearestEnemy(GetPosition()) != nullptr)
	{
		// 一番近い敵を保存
		mEnemy = GetGame()->GetNearestEnemy(GetPosition());
		
		if (mAic->GetCurrentState()->GetName() == "Attack")
		{
			// Attackに切り替え
			mAic->ChangeState("Attack");
		}

		// リキャストタイムを計算
		mNextAttack += AttackTime;
	}
	else
	{
		mAic->ChangeState("Patrol");
	}

	
	if (mNextAttack <= 0.0f)
	{
		// 一番近い敵を取得
		Enemy* e = GetGame()->GetNearestEnemy(GetPosition());
		
		// 敵が存在したら
		if (e != nullptr)
		{
			// 自分から敵に対するベクトルを計算
			Vector2 dir = e->GetPosition() - GetPosition();
			// 敵との距離を計算
			float dist = dir.Length();
			// 攻撃範囲に敵がいたら
			if (dist < AttackRange)
			{
				// 敵の方向に方向転換
				SetRotation(Math::Atan2(-dir.y, dir.x));

				// 塔(自分)の座標にバレットを生成
				Bullet* b = new Bullet(GetGame());
				b->SetPosition(GetPosition());
				b->SetRotation(GetRotation());
			}
		}
		
	}
	*/
}
