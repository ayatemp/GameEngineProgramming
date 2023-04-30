//
//  MainMenu.cpp
//  Game-mac
//
//  Created by 加来彩人 on 2021/12/01.
//  Copyright © 2021 Sanjay Madhav. All rights reserved.
//

#include "MainMenu.hpp"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>

MainMenu::MainMenu(Game* game)
    :UIScreen(game)
{
    mGame->SetState(Game::EMainMenu);
    SetRelativeMouseMode(false);
    SetTitle("MainTitle");
    AddButton("StartButton", [this]() {
        Close();
    });
    AddButton("QuitButton", [this]() {
        new DialogBox(mGame, "QuitText",
            [this]() {
                mGame->SetState(Game::EQuit);
        });
    });
}

MainMenu::~MainMenu()
{
    SetRelativeMouseMode(true);
    mGame->SetState(Game::EGameplay);
}

void MainMenu::HandleKeyPress(int key)
{
    UIScreen::HandleKeyPress(key);
    
    if (key == SDLK_ESCAPE)
    {
        Close();
    }
}
