// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "MeshComponent.h"
#include "MatrixPalette.h"
#include "string"
#include <unordered_map>
#include <iostream>

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	// Draw this mesh component
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);
    
    //　課題１２−２
    void GetGlobalPoseBrending(std::vector<Matrix4> currentPoses);
    void SetSkeletonAnimation(std::string,class Animation*);
    void SetAnimation(std::string AnimeName,Animation* anim);

    
protected:
	void ComputeMatrixPalette();

	MatrixPalette mPalette;
	const class Skeleton* mSkeleton;
	const class Animation* mAnimation;
	float mAnimPlayRate;
	float mAnimTime;
    
    std::unordered_map<std::string, Animation*> AnimationV;
    std::string nowAnimationName;
    std::unordered_map<Animation*,std::string> AnimationName;
    
    float mAnimeTime_Idle;
    float mAnimeTime_RUN;
    float BrendingTime;
};
