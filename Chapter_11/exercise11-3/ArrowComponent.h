//
//  ArrowComponent.h
//  Chapter11-mac
//
//  Created by 加来彩人 on 2021/12/08.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Component.h"

class ArrowComponent : public Component
{
public:
    ArrowComponent(class Actor* owner);
    ~ArrowComponent();
};
