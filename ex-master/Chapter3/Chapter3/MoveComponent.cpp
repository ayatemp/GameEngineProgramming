#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(class Actor* owner, float mass, Vector2 velocityLimit, int updateOrder)
	:Component(owner, updateOrder),
	mAngularSpeed(0.0f),
	mForwardSpeed(Vector2::Zero),
	mMass(mass),
	mVelocityLimit(velocityLimit)
{

}

void MoveComponent::Update(float deltaTime)
{
	// ��]�X�s�[�h��0�ɋ߂���Ίp�x���X�V���Ȃ�
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	// �O�i�X�s�[�h��0�ɋ߂���Έʒu���X�V���Ȃ�
	if (!Math::NearZero(mSumOfForces.Length()))
	{
		/*-------------------------�ύX�����ӏ�-------------------------------------*/

		Vector2 pos = mOwner->GetPosition();

		// �͂̍��v��������x���X�V
		mFowardAcceleration = mSumOfForces * (1.0f / mMass);
		// ���x���X�V
		mForwardSpeed += mFowardAcceleration * deltaTime;

		// ���x�𐧌�����
		// Math.h ������������܂����B
		if (mForwardSpeed > mVelocityLimit)
		{
			mForwardSpeed = mVelocityLimit;
		}

		mSumOfForces = Vector2::Zero;

		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;

		/*-------------------------------�����܂�-----------------------------------*/

		//mForwardSpeed = 0.0f;
		
		// (Screen wrapping code only for asteroids)
		if (pos.x < 0.0f) { pos.x = 1022.0f; }
		else if (pos.x > 1024.0f) { pos.x = 2.0f; }

		if (pos.y < 0.0f) { pos.y = 766.0f; }
		else if (pos.y > 768.0f) { pos.y = 2.0f; }

		mOwner->SetPosition(pos);
	}
}

/*-------------------------�ǉ������ӏ�-------------------------------------*/

void MoveComponent::AddForce(Vector2& force)
{	
	mSumOfForces += force;
}
/*-------------------------------�����܂�-----------------------------------*/
