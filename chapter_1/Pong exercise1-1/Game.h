#include <iostream>
#include "SDL.h"

struct Vector2
{
    float x;
    float y;
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
    
    //グラフィックの描写をするための変数（オブジェクト）
    SDL_Renderer* mRenderer;
    
    Vector2 mPaddlePos;
    Vector2 mBallPos;
    
    //右側のパッド
    Vector2 mPaddlePos_2;
    
    //ボールのx，y方向の進み方を保存する変数
    Vector2 mBallVel;
    
};


