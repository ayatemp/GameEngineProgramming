#pragma once
#include "Component.h"
#include "Math.h"

class MoveComponent : public Component
{
public:
	// UpdateOrder�̒l���������قǁA��ɍX�V�����
	MoveComponent(class Actor* owner, float mass, Vector2 velocityLimit, int updateOrder = 10);

	void Update(float deltaTime) override;

	/*-------------------------�ǉ������ӏ�-------------------------------------*/

	void AddForce(Vector2& force);
	/*-------------------------------�����܂�-----------------------------------*/

	float GetAngularSpeed() const { return mAngularSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	
	/*-------------------------�ύX�����ӏ�-------------------------------------*/
	Vector2 GetForwardSpeed() const { return mForwardSpeed; }
	void SetForwardSpeed(Vector2 speed) { mForwardSpeed = speed; }

	Vector2 GetSumOfForces() const { return mSumOfForces; }
	void SetSumOfForces(Vector2 sumOfFoces) { mSumOfForces = sumOfFoces; }
	/*-------------------------------�����܂�-----------------------------------*/



private:
	// ��]�𐧌䂷�� (���W�A�� / �b)
	float mAngularSpeed;
	
	/*-------------------------�ύX�����ӏ�-------------------------------------*/

	// �O�i�^���𐧌䂷�� (�P�ʒ� / �b)
	Vector2 mForwardSpeed;

	/*-------------------------------�����܂�-----------------------------------*/


	/*-------------------------�ǉ������ӏ�-------------------------------------*/
	// ����
	float mMass;
	// �͂̍��v
	Vector2 mSumOfForces;
	// ���x
	Vector2 mVelocity;
	// �������x
	Vector2 mVelocityLimit;
	// �����x
	Vector2 mFowardAcceleration;
	/*-------------------------------�����܂�-----------------------------------*/
};