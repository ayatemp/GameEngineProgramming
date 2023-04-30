// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "CameraComponent.h"
#include <vector>

struct Spline
{
	// スプラインのための制御点
	// (曲線に属する点がn個ならば
	// n+2個の点が必要)
	std::vector<Vector3> mControlPoints;
	// startIdx=P1の区間で
	// tの値に基づいて位置を計算する
	Vector3 Compute(size_t startIdx, float t) const;
	size_t GetNumPoints() const { return mControlPoints.size(); }
};

// 制御点によってどうやってカメラを動かすのかを考える
class SplineCamera : public CameraComponent
{
public:
	SplineCamera(class Actor* owner);

	void Update(float deltaTime) override;
	// Restart the spline
	void Restart();

	void SetSpeed(float speed) { mSpeed = speed; }
	void SetSpline(const Spline& spline) { mPath = spline; }
	void SetPaused(bool pause) { mPaused = pause; }
private:
	// カメラが追従するスプライン経路
	Spline mPath;
	// 現在の制御点のインデックスとtの値
	size_t mIndex;
	float mT;
	// スピード = tの増分/秒
	float mSpeed;
	// カメラを経路に沿って動かすか
	bool mPaused;
    
    //課題９−２
    bool mRestart;
    bool mC;
};
