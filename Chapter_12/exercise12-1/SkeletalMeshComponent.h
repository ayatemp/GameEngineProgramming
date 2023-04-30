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

#include <vector>
#include <iostream>

class SkeletalMeshComponent : public MeshComponent
{
public:
	SkeletalMeshComponent(class Actor* owner);
	// メッシュコンポーネントを描画
	void Draw(class Shader* shader) override;

	void Update(float deltaTime) override;

	// Setters
	void SetSkeleton(const class Skeleton* sk) { mSkeleton = sk; }

	// Play an animation. Returns the length of the animation
	float PlayAnimation(const class Animation* anim, float playRate = 1.0f);
    
    //課題１２−１
    Matrix4 GetBonePosition(std::string mBoneName);
protected:
	void ComputeMatrixPalette();

    //行列パレット
	MatrixPalette mPalette;
	const class Skeleton* mSkeleton;
    //今再生しているアニメーション
	const class Animation* mAnimation;
    //アニメーションの再生レート(1.0が正常のスピード
	float mAnimPlayRate;
    //アニメーションの現在の時刻
	float mAnimTime;
    
    //課題１２−１ //ポーズ行列を格納するための配列の準備
    std::vector<Matrix4> mPose;
};
