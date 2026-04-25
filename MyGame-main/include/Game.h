#ifndef GAME_H
#define GAME_H
#include "Player.h"
#include "Item.h"
#include "MiniGame.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cctype>

class Game {
    friend class MiniGame;
private:
private:
    Player player;                        // 玩家
    vector<Enemy*> enemies;              // 所有敵人
    bool succeed;                       // 紀錄是否成功畢業
    
    void initEnemies();                // 初始化用
    bool startGame(int gameChoose);   // 啟動特定小遊戲
    bool canGraduate() const;        // 檢查畢業條件
    void printHomePage();
    
    // 畢業條件
    static const int GRADUATE_KNOWLEDGE = 85;
    static const int GRADUATE_FRIENDSHIP = 50;
    static const int GRADUATE_ROMANCE = 80;
    
    // 輸入檢查用
    static const int MIN_GAME_CHOICE = 1;
    static const int MAX_GAME_CHOICE = 6;
    static bool isValidInput(const std::string& input);

public:
    Game (const string& playerName);
    
    void start();
    
    ~Game() {
        for(auto enemy : enemies) {
            delete enemy;
        }
        enemies.clear();
    }
};
#endif
