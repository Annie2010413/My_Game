#ifndef MINIGAME_H
#define MINIGAME_H
#include "Player.h"
#include "Enemy.h"
#include <vector>

// 前向宣告
class Player;
class Enemy;

class MiniGame {
protected:
    Player& player;          
    vector<Enemy*>& enemies;
    
public:
    MiniGame(Player& p, std::vector<Enemy*>& e)
            : player(p), enemies(e) {}
    virtual bool start() = 0;
    virtual void updateStatus(bool win) = 0;
    virtual ~MiniGame() {}
};
#endif
