//
//  SpotLightComponent.h
//  Chapter13-mac
//
//  Created by 加来彩人 on 2021/12/25.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Math.h"
#include "Component.h"

class SpotLightComponent : public Component
{
public:
    SpotLightComponent(class Actor* owner);
    ~SpotLightComponent();

    // Draw this point light as geometry
    void Draw(class Shader* shader, class Mesh* mesh);

    // Diffuse color
    Vector3 mDiffuseColor;
    // Radius of light
    float mInnerRadius;
    float mOuterRadius;
    
    float angle;
};
