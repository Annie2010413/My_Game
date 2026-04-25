#ifndef GIRLFRIEND_H
#define GIRLFRIEND_H
#include "../MiniGame.h"
#include <string>

class Dating : public MiniGame {
private:
    string answer;
    bool answered;

    void pressEnterToContinue();
    void displayLines(const string lines[], int size);
    void getInput();
    void handleQuestion(const string dialogues[], int dialogues_size, const string options[], const string correct_answer);
    void gotGirlfriendHelp();
public:
    Dating(Player& player, vector<Enemy*>& enemies);

    void updateStatus(bool win) override;
    bool start() override;
};

#endif
