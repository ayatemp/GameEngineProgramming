// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"

class MoveComponent : public Component
{
public:
	// Lower update order to update first
	MoveComponent(class Actor* owner, int updateOrder = 10);
	void Update(float deltaTime) override;
	
	float GetAngularSpeed() const { return mAngularSpeed; }
	float GetForwardSpeed() const { return mForwardSpeed; }
	float GetStrafeSpeed() const { return mStrafeSpeed; }
	void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
	void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
	void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }
    
    //課題１０−１
    void SetJumpSpeed(float speed) { mJumpSpeed = speed; }
    float GetJumpSpeed() { return mJumpSpeed; }
    void ResetTime() { Time = 0.0; }
protected:
	float mAngularSpeed;
	float mForwardSpeed;
	float mStrafeSpeed;
    
    //課題１０−１
    float mJumpSpeed;
    float Time;
};
