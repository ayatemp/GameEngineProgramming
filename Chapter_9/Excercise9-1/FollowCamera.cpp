// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowCamera.h"
#include "Actor.h"

FollowCamera::FollowCamera(Actor* owner)
	:CameraComponent(owner)
	,mHorzDist(350.0f)
	,mVertDist(150.0f)
	,mTargetDist(100.0f)
	,mSpringConstant(64.0f)
    ,mUp(Vector3::UnitZ)
    ,mPitchSpeed(0.0f)
    ,mYawSpeed(0.0f)
{
    mOffset = ComputeCameraPos();
}

void FollowCamera::Update(float deltaTime)
{
	CameraComponent::Update(deltaTime);
    
    // 課題９−１-----------------------------------------------
    if(OrbitMode)
    {
        //mOffset = mActualPos;
        Quaternion yaw(Vector3::UnitZ, mYawSpeed * deltaTime);
        // Transform offset and up by yaw
        mOffset = Vector3::Transform(mOffset, yaw);
        mUp = Vector3::Transform(mUp, yaw);

        // Compute camera forward/right from these vectors
        // Forward owner.position - (owner.position + offset)
        // = -offset
        Vector3 forward = -1.0f * mOffset;
        forward.Normalize();
        Vector3 right = Vector3::Cross(mUp, forward);
        right.Normalize();
        
        // Create quaternion for pitch about camera right
        Quaternion pitch(right, mPitchSpeed * deltaTime);
        // Transform camera offset and up by pitch
        mOffset = Vector3::Transform(mOffset, pitch);
        mUp = Vector3::Transform(mUp, pitch);

        // Compute transform matrix
        Vector3 target = mOwner->GetPosition()+mOwner->GetForward() * mTargetDist;;
        Vector3 cameraPos =  mOffset;
        Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, mUp);
        SetViewMatrix(view);
        OrbitMode = false;
        mActualPos = mOffset;
    }
    else
    {
    //-------------------------------------------------------
	// バネ定数から減衰を計算
	float dampening = 2.0f * Math::Sqrt(mSpringConstant);
	// 理想の位置を計算
	Vector3 idealPos = ComputeCameraPos();
	// 十歳と理想の差を計算
	Vector3 diff = mActualPos - idealPos;
	// バネによる加速度を計算
	Vector3 acel = -mSpringConstant * diff -
		dampening * mVelocity;
	// 速度の更新
	mVelocity += acel * deltaTime;
	// 実際のカメラポジションを更新
	mActualPos += mVelocity * deltaTime;
	// ターゲットは所有アクターの前方に離れた目標点
	Vector3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// ここでは理想ではなく実際のポジションを使う
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target,
		Vector3::UnitZ);
	SetViewMatrix(view);
        
        
    }
    
}

void FollowCamera::SnapToIdeal()
{
	// Set actual position to ideal
	mActualPos = ComputeCameraPos();
	// Zero velocity
	mVelocity = Vector3::Zero;
	// Compute target and view
	Vector3 target = mOwner->GetPosition() +
		mOwner->GetForward() * mTargetDist;
	// Use actual position here, not ideal
	Matrix4 view = Matrix4::CreateLookAt(mActualPos, target,
		Vector3::UnitZ);
	SetViewMatrix(view);
}

Vector3 FollowCamera::ComputeCameraPos() const
{
	// Set camera position behind and above owner
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}

void FollowCamera::OrbitInitialize()
{
    mOffset = ComputeCameraPos();
    mUp = Vector3::UnitZ;
    mPitchSpeed = 0.0f;
    mYawSpeed = 0.0f;
}
