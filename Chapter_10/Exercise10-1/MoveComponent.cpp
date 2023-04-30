// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"

#include <iostream>

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
:Component(owner, updateOrder)
,mAngularSpeed(0.0f)
,mForwardSpeed(0.0f)
,Time(0.0f)
{
	
}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rot = mOwner->GetRotation();
		float angle = mAngularSpeed * deltaTime;
		// Create quaternion for incremental rotation
		// (Rotate about up axis)
		Quaternion inc(Vector3::UnitZ, angle);
		// Concatenate old and new quaternion
		rot = Quaternion::Concatenate(rot, inc);
		mOwner->SetRotation(rot);
	}
	
	if (!Math::NearZero(mForwardSpeed) || !Math::NearZero(mStrafeSpeed) || !Math::NearZero(mJumpSpeed))
	{
        Time += deltaTime;
		Vector3 pos = mOwner->GetPosition();
		pos += mOwner->GetForward() * mForwardSpeed * deltaTime;
		pos += mOwner->GetRight() * mStrafeSpeed * deltaTime;
        //pos.z = 400 * Time - 1 * 9.81 * Time * Time * 100  / 2 ;
        
        //std::cout<<"pos : "<<pos.x<<" "<<pos.y<<" "<<pos.z;
        std::cout<<"  Time : "<<Time<<std::endl;
		mOwner->SetPosition(pos);
	}
    
    //課題１０−１
    
    if(!Math::NearZero(mJumpSpeed))
    {
        Time += deltaTime;
        Vector3 pos = mOwner->GetPosition();
        //x = 1/2*at^2 +vt +xを実装
        //GetJumpはどの軸の方向にプラスしてくれるのかを調べてくれる
        pos.z = 400 * Time - 1 * 9.81 * Time * Time * 30  / 2 ;
        
        std::cout<<"pos : "<<pos.x<<" "<<pos.y<<" "<<pos.z<<std::endl;
        std::cout<<"  Time : "<<Time<<std::endl;
        mOwner->SetPosition(pos);
    }
    
    
        
}
