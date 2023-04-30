// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "PointLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Actor.h"

PointLightComponent::PointLightComponent(Actor* owner)
	:Component(owner)
{
	owner->GetGame()->GetRenderer()->AddPointLight(this);
}

PointLightComponent::~PointLightComponent()
{
	mOwner->GetGame()->GetRenderer()->RemovePointLight(this);
}

void PointLightComponent::Draw(Shader* shader, Mesh* mesh)
{
	// We assume, coming into this function, that the shader is active
	// and the sphere mesh is active

	//　ワールド変換は，外側の半径によってスケーリングして
    //　（ただしメッシュの半径で割って）ワールド空間に置く
	Matrix4 scale = Matrix4::CreateScale(mOwner->GetScale() *
		mOuterRadius / mesh->GetRadius());
	Matrix4 trans = Matrix4::CreateTranslation(mOwner->GetPosition());
	Matrix4 worldTransform = scale * trans;
	shader->SetMatrixUniform("uWorldTransform", worldTransform);
	// 点光源シェーダーの定数を設定
	shader->SetVectorUniform("uPointLight.mWorldPos", mOwner->GetPosition());
	shader->SetVectorUniform("uPointLight.mDiffuseColor", mDiffuseColor);
	shader->SetFloatUniform("uPointLight.mInnerRadius", mInnerRadius);
	shader->SetFloatUniform("uPointLight.mOuterRadius", mOuterRadius);
    shader->SetVectorUniform("uPointLight.mSpecColor", mSpecColor);
    shader->SetFloatUniform("uPointLight.mSpecPower", 100.0);

	// 球を描画
	glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(), 
		GL_UNSIGNED_INT, nullptr);
}
