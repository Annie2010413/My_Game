#ifndef TUTOR_H
#define TUTOR_H
#include "../MiniGame.h"

class Tutor : public MiniGame{
private:
public:
    Tutor(Player& player, vector<Enemy*>& enemies); //constructor
    void updateStatus(bool win) override;
    bool start() override;
};
#endif
