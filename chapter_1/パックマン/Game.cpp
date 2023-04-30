//
//  Game.cpp
//  パックマン
//
//  Created by 加来彩人 on 2021/07/05.
//

#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mwindow(nullptr)
,mRenderer(nullptr)
,mIsRunnnig(true)
,mTicksCount(0)
,mPaddleDir(0)
{
    
}


//全ての初期化の関数
int Game::Initialize(){
    
    //SDL＿INIT関数で初期化する
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    
    if(sdlResult != 0){
        //C言語のprintfと同じようにコンソールに出力してくれる関数
        SDL_Log("SDLを初期化できません：%s",SDL_GetError());
        return 1;
    }
    
    
    mwindow = SDL_CreateWindow("Game Programming in C++（第一章）", 100, 100, 1024, 768, 0);
    
    //画面が作れなかった時のためのエラー文作成
    if(!mwindow){
        SDL_Log("ウィンドウの作成に失敗しました : %s",SDL_GetError());
        return 1;
    }
    
    //SDLレンダラーの作成
    mRenderer = SDL_CreateRenderer(mwindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    
    
    mBallPos.x = 1024.0f/2.0f;
    mBallPos.y = 768.0f/2.0f;
    
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f/2.0f;
    
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;
    
    return 0;
}

void Game::Shutdown(){
    SDL_DestroyWindow(mwindow);
    SDL_Quit();
    SDL_DestroyRenderer(mRenderer);
}


void Game::RunLoop(){
    while(mIsRunnnig){
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    //キューにイベントがあれば繰り返す
    while (SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_QUIT:
                mIsRunnnig = false;
                break;
        }
    }
    
    //キーボードの状態を取得する
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    //[ESC]キーが押されていても，ループを終える
    if(state[SDL_SCANCODE_ESCAPE]){
        mIsRunnnig = false;
    }
    
    mPaddleDir = 0;
    if(state[SDL_SCANCODE_W])
    {
        mPaddleDir -=1;
    }
    if(state[SDL_SCANCODE_S])
    {
        mPaddleDir +=1;
    }
}

void Game::GenerateOutput(){
    SDL_SetRenderDrawColor
    (
     mRenderer,
     0,//R
     0,//G
     255,//B
     255//A
     );
    
    SDL_RenderClear(mRenderer);
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    
    SDL_Rect wall{
        0,
        0,
        1024,
        thickness
    };
    
    SDL_RenderFillRect(mRenderer, &wall);
    
    SDL_Rect paddle{
            static_cast<int>(mPaddlePos.x),
            static_cast<int>(mPaddlePos.y - paddleH/2),
            thickness,
            static_cast<int>(paddleH)
        };
        SDL_RenderFillRect(mRenderer, &paddle);
    
    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    SDL_Rect ball{
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    
    SDL_RenderFillRect(mRenderer, &ball);
    
    SDL_RenderPresent(mRenderer);
}

void Game::UpdateGame(){
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
    
    //deltatimeは前のフレームとの時刻の差を秒に変換した値
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    //時刻を更新
    mTicksCount = SDL_GetTicks();
    
    if(deltaTime>0.05f){
        deltaTime = 0.05f;
    }
    //ゲームワールドのオブジェクトをデルタタイムとして更新する！
    if(mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        if(mPaddlePos.y < (paddleH/2.0f + thickness))
        {
            mPaddlePos.y = paddleH/2.0f + thickness;
        }
        else if (mPaddlePos.y > (768.0f - paddleH/2.0f - thickness))
        {
            mPaddlePos.y = 768.0 - paddleH/2.0f - thickness;
        }
    }
    
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;
    
    float diff = mPaddlePos.y - mBallPos.y;
    // Take absolute value of difference
    diff = (diff > 0.0f) ? diff : -diff;
        
    if (
            // Our y-difference is small enough
            diff <= paddleH / 2.0f &&
            // We are in the correct x-position
            mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
            // The ball is moving to the left
            mBallVel.x < 0.0f)
        {
            mBallVel.x *= -1.0f;
        }
        // Did the ball go off the screen? (if so, end game)
        else if (mBallPos.x <= 0.0f)
        {
            mIsRunnnig = false;
        }
        // Did the ball collide with the right wall?
        else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
        {
            mBallVel.x *= -1.0f;
        }
        
        // Did the ball collide with the top wall?
        if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
        {
            mBallVel.y *= -1;
        }
        // Did the ball collide with the bottom wall?
        else if (mBallPos.y >= (768 - thickness) &&
            mBallVel.y > 0.0f)
        {
            mBallVel.y *= -1;
        }
}

