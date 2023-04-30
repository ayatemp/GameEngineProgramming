//
//  SpotLightComponent.cpp
//  Game-mac
//
//  Created by 加来彩人 on 2021/12/25.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "SpotLightComponent.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Mesh.h"
#include "VertexArray.h"
#include "Actor.h"
#include "Math.h"

SpotLightComponent::SpotLightComponent(Actor* owner)
    :Component(owner)
{
    owner->GetGame()->GetRenderer()->AddSpotLight(this);
}

SpotLightComponent::~SpotLightComponent()
{
    mOwner->GetGame()->GetRenderer()->RemoveSpotLight(this);
}

void SpotLightComponent::Draw(Shader* shader, Mesh* mesh)
{
    Vector3 mPositionAngle = mOwner->GetPosition() ;
    
    Matrix4 scaleAngle = Matrix4::CreateScale(1, angle/30, angle/30);
    Matrix4 scale = Matrix4::CreateScale(mOwner->GetScale() *
        mOuterRadius / mesh->GetRadius() );
    Matrix4 trans = Matrix4::CreateTranslation(mPositionAngle);
    Matrix4 worldTransform = scaleAngle * scale * trans ;
    shader->SetMatrixUniform("uWorldTransform", worldTransform);
    // Set point light shader constants
    shader->SetVectorUniform("uSpotLight.mWorldPos", mPositionAngle);
    shader->SetVectorUniform("uSpotLight.mDiffuseColor", mDiffuseColor);
    shader->SetFloatUniform("uSpotLight.mInnerRadius", mInnerRadius);
    shader->SetFloatUniform("uSpotLight.mOuterRadius", mOuterRadius);

    shader->SetFloatUniform("uSpotLight.mAngle",angle);
    // Draw the sphere
    glDrawElements(GL_TRIANGLES, mesh->GetVertexArray()->GetNumIndices(),
        GL_UNSIGNED_INT, nullptr);
}

