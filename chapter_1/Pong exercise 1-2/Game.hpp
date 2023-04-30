#include <iostream>
#include <vector>
#include <random>
#include "SDL.h"

struct Vector2
{
    float x;
    float y;
};

struct Ball
{
    Vector2 Pos;
    Vector2 Vel;
};

class Game{
public:
    Game();
    //ゲームを初期化する
    int Initialize();
    //ゲームオーバーまでゲームループを実行する
    void RunLoop();
    //ゲームをシャットダウンする
    void Shutdown();
private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    
    //SDLが作るウィンドウ
    SDL_Window* mwindow;
    //デルタタイム変数
    Uint32 mTicksCount;
    //ゲームの続行を指示する
    bool mIsRunnnig;
    
    //パドル（ラケットの位置）
    int mPaddleDir;
    int mPaddleDir_2;
    Vector2 mPaddlePos;
    Vector2 mPaddlePos_2;
    
    //グラフィックの描写をするための変数（オブジェクト）
    SDL_Renderer* mRenderer;
    
    //ボールの位置と進む方向の入力
    Vector2 mBallPos;
    Vector2 mBallVel;
    
    std::vector<Ball> wBall;
    
};


