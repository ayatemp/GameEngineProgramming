#include "Asteroid.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "Random.h"
#include "CircleComponent.h"
#include "Math.h"

Asteroid::Asteroid(Game* game)
	:Actor(game),
	mCircle(nullptr)
{
	// �����_���Ȉʒu / �����ŏ���������
	Vector2 randPos = Random::GetVector(Vector2::Zero, Vector2(1024.0f, 768.0f));
	
	SetPosition(randPos);
	SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

	// �X�v���C�g�R���|�[�l���g���쐬���A�e�N�X�`����ݒ肷��
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Asteroid.png"));
	
	/*-------------------------�ǉ������ӏ�-------------------------------------*/

	// �^���R���|�[�l���g���쐬���A�O�i���x��ݒ肷��
	mMove = new MoveComponent(this, 0.1f, Vector2(140.0f, 140.0f));
	Vector2 force = Vector2(10.0f, 10.0f);
	mMove->AddForce(force);
	//mc->SetForwardSpeed(150.0f);
	/*-------------------------------�����܂�-----------------------------------*/

	// �Փ˔���̂��߂ɃT�[�N���R���|�[�l���g���쐬
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(40.0f);

	game->AddAsteroid(this);
}

Asteroid::~Asteroid()
{
	GetGame()->RemoveAsteroid(this);
}

/*-------------------------�ǉ������ӏ�-------------------------------------*/

void Asteroid::UpdateActor(float deltaTime)
{
	Vector2 force = Vector2(1.0f, 1.0f);
	mMove->AddForce(force);
}
/*-------------------------------�����܂�-----------------------------------*/
