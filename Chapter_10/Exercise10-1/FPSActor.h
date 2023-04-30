// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "SoundEvent.h"

enum Jump_State
{
    Ground,
    Jump_now,
    Fall
};

class FPSActor : public Actor
{
public:
	FPSActor(class Game* game);

	void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keys) override;

	void Shoot();

	void SetFootstepSurface(float value);

	void SetVisible(bool visible);

	void FixCollisions();
    
    //課題１０−１
    void Jump();
    void SetJumpState(Jump_State State){ mstate = State; };
private:
	class MoveComponent* mMoveComp;
	class AudioComponent* mAudioComp;
	class MeshComponent* mMeshComp;
	class FPSCamera* mCameraComp;
	class BoxComponent* mBoxComp;
	class Actor* mFPSModel;
	SoundEvent mFootstep;
	float mLastFootstep;
    
    //課題１０−１
    float mZSpeed;
    float mGravity;
    Jump_State mstate;
};
