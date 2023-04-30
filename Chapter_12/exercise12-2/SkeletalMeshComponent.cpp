// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "Actor.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include <string>

SkeletalMeshComponent::SkeletalMeshComponent(Actor* owner)
	:MeshComponent(owner, true)
	,mSkeleton(nullptr)
{
}

void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// Set the world transform
		shader->SetMatrixUniform("uWorldTransform", 
			mOwner->GetWorldTransform());
		// Set the matrix palette
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0], 
			MAX_SKELETON_BONES);
		// Set specular power
		shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
		// Set the active texture
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}
		// Set the mesh's vertex array as active
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		// Draw
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;
        BrendingTime = mAnimTime;
		// Wrap around anim time if past duration
		while (mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime -= mAnimation->GetDuration();
		}

		// Recompute matrix palette
		ComputeMatrixPalette();
	}
}

float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)
{
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;
    BrendingTime = 0.0f;
    
    //課題１２−２　ブレンディングの導入（アニメーション同士の融合）
    //やりたいこと
    //アニメーションの静止状態と動いてる状態の区別がはっきりしすぎているので
    //二つの間のアニメーションをブレンディングタイムを使って作りたい

	if (!mAnimation) { return 0.0f; }
    

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}

void SkeletalMeshComponent::ComputeMatrixPalette()
{
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();
	std::vector<Matrix4> currentPoses;  //現在のポーズ
    //切り替えするための処理をここでしたい
    //ここで両方のアニメーションのポーズを計算する
    if(BrendingTime >= 0.0f && BrendingTime < 2.0f)
    {
        
    }
    
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);
    
    
	// Setup the palette for each bone
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// Global inverse bind pose matrix times current pose matrix
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}

void SkeletalMeshComponent::SetAnimation(std::string AnimeName,Animation* anim)
{
    AnimationName[anim] = AnimeName;
}

//
void SkeletalMeshComponent::GetGlobalPoseBrending(std::vector<Matrix4> outPoses)
{
    Animation* anim1 = AnimationV["CatActionIdle"];
    Animation* anim2 = AnimationV["CatRunSprint"];
    
    float mNumBones1 = anim1->GetNumBones();
    float mNumBones2 = anim2->GetNumBones();
    
    if (outPoses.size() != mNumBones1)
    {
        outPoses.resize(mNumBones1);
    }

    // Figure out the current frame index and next frame
    // (This assumes inTime is bounded by [0, AnimDuration]
    size_t frame1 = static_cast<size_t>(BrendingTime / anim1->GetFrameDuration());
    size_t nextFrame = frame1 + 1;
    // Calculate fractional value between frame and next frame
    float pct = BrendingTime / anim1->GetFrameDuration() - frame1;
    std::vector<std::vector<BoneTransform>> mTracks1 = anim1->GetBoneTransform();

    // Setup the pose for the root
    if (mTracks1[0].size() > 0)
    {
        // Interpolate between the current frame's pose and the next frame
        BoneTransform interp = BoneTransform::Interpolate(mTracks1[0][frame1],
            mTracks1[0][nextFrame], pct);
        outPoses[0] = interp.ToMatrix();
    }
    else
    {
        outPoses[0] = Matrix4::Identity;
    }

    const std::vector<Skeleton::Bone>& bones = mSkeleton->GetBones();
    // Now setup the poses for the rest
    for (size_t bone = 1; bone < mNumBones1; bone++)
    {
        Matrix4 localMat; // (Defaults to identity)
        if (mTracks1[bone].size() > 0)
        {
            BoneTransform interp = BoneTransform::Interpolate(mTracks1[bone][frame1],
                mTracks1[bone][nextFrame], pct);
            localMat = interp.ToMatrix();
        }

        outPoses[bone] = localMat * outPoses[bones[bone].mParent];
    }
}

void SkeletalMeshComponent::SetSkeletonAnimation(std::string animeName, Animation* anim)
{
    AnimationV[animeName] = anim;
}
