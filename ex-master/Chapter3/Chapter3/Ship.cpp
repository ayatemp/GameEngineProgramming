// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "Asteroid.h"

Ship::Ship(Game* game)
	:Actor(game)
	, mLaserCooldown(0.0f)
	, mShipCooldown(2.0f)
	, mCircle(nullptr)
{
	// �X�v���C�g�R���|�[�l���g���쐬
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// �C���v�b�g�R���|�[�l���g�ƃL�[�ƃX�s�[�h���쐬
	InputComponent* ic = new InputComponent(this, 1.0f, Vector2(200.0f, 200.0f));
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	// �T�[�N���R���|�[�l���g���쐬
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);
}

void Ship::UpdateActor(float deltaTime)
{
	mLaserCooldown -= deltaTime;

	/*-------------------------�ǉ������ӏ�-------------------------------------*/
	
	// ��Ԃ�EActive�Ȃ�Փ˔�����v�Z
	if (GetState() == EActive)
	{
		// �A�X�e���C�h�ƏՓ˂������𔻒�
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				// �Փ˂������Ԃ�EPaused�ɐݒ�
				SetState(EPaused);
				break;
			}
		}
	}
	
	// ��Ԃ�EPaused�Ȃ烊�X�|�[�����邩���v�Z
	if (GetState() == EPaused)
	{	
		// ���X�|�[���̃N�[���_�E�����Ԃ��v�Z
		mShipCooldown -= deltaTime;
		
		// �N�[���_�E�����ԂɂȂ�����ship�������ʒu�E�p�x�Ƀ��X�|�[�� 
		if (mShipCooldown <= 0.0f)
		{
			SetPosition(Vector2(512.0f, 384.0f));
			SetRotation(Math::PiOver2);
			SetState(EActive);
			mShipCooldown = 2.f;
		}
	}
	/*-------------------------------�����܂�-----------------------------------*/
}

void Ship::ActorInput(const uint8_t* keyState)
{
	if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		// Create a laser and set its position/rotation to mine
		Laser* laser = new Laser(GetGame());
		laser->SetPosition(GetPosition());
		laser->SetRotation(GetRotation());

		// Reset laser cooldown (half second)
		mLaserCooldown = 0.5f;
	}
}
