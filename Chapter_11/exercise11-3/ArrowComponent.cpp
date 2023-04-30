//
//  ArrowComponent.cpp
//  Game-mac
//
//  Created by 加来彩人 on 2021/12/08.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "BoxComponent.h"
#include "Actor.h"
#include "Game.h"
#include "PhysWorld.h"
#include "ArrowComponent.h"
#include "Math.h"
#include "Game.h"
#include "HUD.h"

ArrowComponent::ArrowComponent(Actor * owner)
    :Component(owner)
{
    mOwner->GetGame()->GetHUD()->AddArrowComponent(this);
}

ArrowComponent::~ArrowComponent()
{
    mOwner->GetGame()->GetHUD()->RemoveArrowComponent(this);
}
