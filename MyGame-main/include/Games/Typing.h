#ifndef TYPING_H
#define TYPING_H
#include <SFML/Graphics.hpp>
#include "../MiniGame.h"
#include <string>

// Typing 遊戲類別：模擬 WebWork 作業系統的打字遊戲
class Typing : public MiniGame {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Texture questionTexture;      // 題目圖片
    sf::Sprite questionSprite;
    std::string correctAnswer;
    bool isTimeFrozen;
    sf::Clock freezeTimer;           // 計時器
    
    std::string formatTime(const sf::Time& timeLeft);  // 格式化剩餘時間
    void printGameRules();
    void checkAndUseGirlfriendHelp();
    void handleStudentAttack();
    
    bool usingItem;

public:
    Typing(Player& player, std::vector<Enemy*>& enemies);
    void updateStatus(bool win) override;
    bool start() override;                              
};
#endif
