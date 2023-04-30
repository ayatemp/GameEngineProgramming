//
//  Game.cpp
//  パックマン
//
//  Created by 加来彩人 on 2021/07/05.
//

#include "Game.hpp"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game()
:mwindow(nullptr)
,mRenderer(nullptr)
,mIsRunnnig(true)
,mTicksCount(0)
,mPaddleDir(0)
,mPaddleDir_2(0)
,wBall(3)
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

    
    
    
    //二つのパッドの位置の宣言
    mPaddlePos.x = 10.0f;
    mPaddlePos.y = 768.0f/2.0f;
    mPaddlePos_2.x = 1024.0f-10.0f-(float)thickness;
    mPaddlePos_2.y = 768.0f/2.0f;
    
    //ボールのx，yの速度方向
    mBallPos.x = 1024.0f/2.0f;
    mBallPos.y = 768.0f/2.0f ;
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;
    
    for(int i=0;i<3;i++){
        wBall[i].Pos.x=1024.0f/2.0f;
        wBall[i].Pos.y=768.0f/2.0f - 150.0f*(float)i ;
        wBall[i].Vel.x = -200.0f + 50.0f*(float)i;
        wBall[i].Vel.y = 235.0f + 50.0f*(float)i;
        
        std::cout<<i+1<<"回目"<<std::endl;
        std::cout<<wBall[i].Pos.x<<std::endl;
        std::cout<<wBall[i].Pos.y<<std::endl;
        std::cout<<wBall[i].Vel.x<<std::endl;
        std::cout<<wBall[i].Vel.y<<std::endl;
    }
    
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
    
    //パッドがwなら上へ，sなら下に向かう（実際の更新はUpdateで計算する）
    mPaddleDir = 0;
    if(state[SDL_SCANCODE_W])
    {
        mPaddleDir -=1;
    }
    if(state[SDL_SCANCODE_S])
    {
        mPaddleDir +=1;
    }
    
    mPaddleDir_2 = 0;
    if(state[SDL_SCANCODE_I])
    {
        mPaddleDir_2 -=1;
    }
    if(state[SDL_SCANCODE_K])
    {
        mPaddleDir_2 +=1;
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
    
    //上の壁
    SDL_Rect wall{
        0,
        0,
        1024,
        thickness
    };
    //バックバッファーに入力
    SDL_RenderFillRect(mRenderer, &wall);
    
    
    //パッドの生成(2)
    SDL_Rect paddle{
            static_cast<int>(mPaddlePos.x),
            static_cast<int>(mPaddlePos.y - paddleH/2),
            thickness,
            static_cast<int>(paddleH)
        };
    SDL_RenderFillRect(mRenderer, &paddle);
    
    SDL_Rect paddle_2{
        static_cast<int>(mPaddlePos_2.x),
        static_cast<int>(mPaddlePos_2.y - paddleH/2),
        thickness,
        static_cast<int>(paddleH)
    };
    SDL_RenderFillRect(mRenderer,&paddle_2);
    
    //ボールの生成
    SDL_Rect ball{
        static_cast<int>(mBallPos.x - thickness/2),
        static_cast<int>(mBallPos.y - thickness/2),
        thickness,
        thickness
    };
    //バックバッファーに更新
    SDL_RenderFillRect(mRenderer, &ball);
    
    //三つのボールの生成
    for(int i=0;i<3;i++){
        SDL_Rect ball{
            static_cast<int>(wBall[i].Pos.x - thickness/2),
            static_cast<int>(wBall[i].Pos.y - thickness/2),
            thickness,
            thickness
        };
        
        //バックバッファーに更新
        SDL_RenderFillRect(mRenderer, &ball);
    }
     
    
    //バックバッファーとフロントバァッファーを交換（画面に更新）
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
    
    if(mPaddleDir_2 !=0)
    {
        mPaddlePos_2.y += mPaddleDir_2 * 300.0f *deltaTime;
        if(mPaddlePos_2.y < (paddleH/2.0f + thickness))
        {
            mPaddlePos_2.y = paddleH/2.0f + thickness;
        }
        else if (mPaddlePos_2.y > (768.0f - paddleH/2.0f - thickness))
        {
            mPaddlePos_2.y = 768.0 - paddleH/2.0f - thickness;
        }
    }
    
    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;
    
    for(int i=0;i<3;i++){
        wBall[i].Pos.x += wBall[i].Vel.x * deltaTime;
        wBall[i].Pos.y += wBall[i].Vel.y * deltaTime;
        
        float diff = mPaddlePos.y - wBall[i].Pos.y;
        // Take absolute value of difference
        diff = (diff > 0.0f) ? diff : -diff;
        
        float diff_2 = mPaddlePos_2.y - wBall[i].Pos.y;
        diff_2 = (diff_2 > 0.0f) ? diff_2 : -diff_2;
            
        if (
              // 二つの差がパッドの半分の小ささ以下の時
              diff <= paddleH / 2.0f &&
              //
              wBall[i].Pos.x <= 25.0f && wBall[i].Pos.x >= 20.0f &&
              // The ball is moving to the left
              wBall[i].Vel.x < 0.0f)
            {
                wBall[i].Vel.x *= -1.0f;
            }
            // Did the ball go off the screen? (if so, end game)
        else if (wBall[i].Pos.x <= 0.0f)
            {
                mIsRunnnig = false;
            }
            // Did the ball collide with the right wall?
        
        
        if(
           diff_2 <= paddleH / 2.0f &&
           wBall[i].Pos.x >= 1024.0f - 25.0f  && wBall[i].Pos.x <= 1024.0f - 20.0f  &&
           wBall[i].Vel.x > 0.0f
           )
            {
                wBall[i].Vel.x *= -1.0f;
            }
        else if(wBall[i].Pos.x >= 1024.0f)
        {
            mIsRunnnig = false;
        }
            
            // Did the ball collide with the top wall?
        if (wBall[i].Pos.y <= thickness && wBall[i].Vel.y < 0.0f)
            {
                wBall[i].Vel.y *= -1;
            }
            // Did the ball collide with the bottom wall?
        else if (wBall[i].Pos.y >= (768 - thickness) &&
                wBall[i].Vel.y > 0.0f)
            {
                wBall[i].Vel.y *= -1;
            }
    }
    
    
    float diff = mPaddlePos.y - mBallPos.y;
    // Take absolute value of difference
    diff = (diff > 0.0f) ? diff : -diff;
    
    float diff_2 = mPaddlePos_2.y - mBallPos.y;
    diff_2 = (diff_2 > 0.0f) ? diff_2 : -diff_2;
        
    if (
          // 二つの差がパッドの半分の小ささ以下の時
          diff <= paddleH / 2.0f &&
          //
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
    
    
    if(
       diff_2 <= paddleH / 2.0f &&
       mBallPos.x >= 1024.0f - 25.0f  && mBallPos.x <= 1024.0f - 20.0f  &&
       mBallVel.x > 0.0f
       )
        {
            mBallVel.x *= -1.0f;
        }
    else if(mBallPos.x >= 1024.0f)
    {
        mIsRunnnig = false;
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

