//
//  Main.cpp
//  パックマン
//
//  Created by 加来彩人 on 2021/07/05.
//

#include <stdio.h>
#include "Game.hpp"

int main(int argc,char **argv){
    Game game;
    int success = game.Initialize();
    
    std::cout<<"success : "<<success<<std::endl;
    
    //std::cout<<"OK"<<std::endl;
    
    if(success == 0){
        game.RunLoop();
    }
     
    game.Shutdown();
    return 0;
}
