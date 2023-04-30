// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Math.h"
#include "Component.h"

class PointLightComponent : public Component
{
public:
	PointLightComponent(class Actor* owner);
	~PointLightComponent();

	// 点光源をジオメトリとして描画する
	void Draw(class Shader* shader, class Mesh* mesh);

	// 拡散色
	Vector3 mDiffuseColor;
	// 光の半径
	float mInnerRadius;
	float mOuterRadius;
    //鏡面色
    Vector3 mSpecColor = Vector3(0.8f,0.8f,0.8f);
};
