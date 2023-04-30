//
//  ArrowTarget.h
//  Chapter11-mac
//
//  Created by 加来彩人 on 2021/12/08.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#pragma once
#include "Actor.h"

class ArrowTarget : public Actor
{
public:
    ArrowTarget(class Game* game);
    class BoxComponent* GetBox() { return mBox; }
private:
    class BoxComponent* mBox;
};
