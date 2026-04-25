#ifndef CARDMATCH_H
#define CARDMATCH_H
#include <SFML/Graphics.hpp>
#include "../MiniGame.h"
#include <vector>
#include <memory>
#include <random>

// 卡片類別，繼承自 SFML 的 RectangleShape
class Card : public sf::RectangleShape {
private:
    int id;                     // 卡片唯一 ID
    int matchId;                // 配對 ID (相同的可以配對)
    bool isRevealed;            // 是否翻開
    bool isMatched;             // 是否已配對成功
    sf::Texture frontTexture;   // 卡片正面圖案
    sf::Texture backTexture;    // 卡片背面圖案
    sf::Shader* effectShader;   // 特效著色器

public:
    Card(int cardId, int mId, const std::string& frontImagePath);
    bool operator==(const Card& other) const;    // 用於配對卡片
    void reveal();              // 翻開卡片
    void hide();                // 蓋住卡片
    void match();               // 標記配對成功
    int getId() const;          // 取得卡片 ID
    int getMatchId() const;     // 取得配對 ID
    bool getIsRevealed() const; // 檢查是否翻開
    bool getIsMatched() const;  // 檢查是否配對
    void setPosition(float x, float y);  // 設定卡片位置
    void applyEffect(sf::Shader* shader); // 套用視覺特效
};

class CardMatch : public MiniGame {
private:
    sf::RenderWindow window;
    std::vector<std::unique_ptr<Card>> cards;
    size_t firstSelectedId;     // 第一張選中的卡片ID
    size_t secondSelectedId;    // 第二張選中的卡片ID
    int remainingPairs;
    float timeLimit;
    sf::Clock gameClock;        // 遊戲計時器
    sf::Clock waitingClock;     // 等待計時器
    sf::Font font;
    sf::Text timerText;
    sf::Text instructionText;
    bool isGameActive;
    bool gameStarted;
    bool waitingForWin = false;
    sf::Text gameOverText;
    bool gameResult;
    sf::Shader grayscaleShader; // 灰階特效著色器

    void initializeCards();
    void handleMouseClick(const sf::Event::MouseButtonEvent& mouseButton);
    void selectCard(size_t index);
    void handleEvents();
    void checkMatch();
    void update();              // 更新遊戲狀態
    void render();              // 繪製畫面
    void showInstructions();
    bool checkSpecialConditions(); // 檢查特殊條件

public:
    CardMatch(Player& p, std::vector<Enemy*>& e);
    bool start() override;
    void updateStatus(bool win) override; 
};

#endif
