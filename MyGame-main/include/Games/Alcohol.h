#ifndef ALCOHOL_H
#define ALCOHOL_H
#include "../MiniGame.h"
#include <string>

class Alcohol : public MiniGame{
private:
    const int MAX_ROUND_CNT = 3 ;
    // pss 是 paperScissorStone 的縮寫
    string pssGetChoiceName(int choice);
    int pssDetermineWinner(int userChoice, int computerChoice);
    bool paperScissorStone();
    string alcGetChoiceName(int choice); // 給拚酒用的函式
    int chooseAlcohol(int drunkLevel);
    void gotHammar() ; // 額外事件撿到槌子
public:
    Alcohol(Player& player, vector<Enemy*>& enemies): MiniGame(player, enemies) {} ; // constructor
    bool start() override ;
    void updateStatus(bool win) override;
};
#endif
