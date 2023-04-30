//
//  MainMenu.hpp
//  Game-mac
//
//  Created by 加来彩人 on 2021/12/01.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//


#pragma once
#include "UIScreen.h"

class MainMenu : public UIScreen
{
public:
    MainMenu(class Game* game);
    ~MainMenu();
    
    void HandleKeyPress(int key) override;
};
