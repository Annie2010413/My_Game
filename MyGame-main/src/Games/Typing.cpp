#include "Typing.h"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace std;

Typing::Typing(Player& player, vector<Enemy*>& enemies)
: MiniGame(player, enemies), isTimeFrozen(false), usingItem(false) {
    
    correctAnswer = "-10(xln|x^2-1|-2x-(ln|(x-1)/(x+1)|))+C";
    window.create(sf::VideoMode(800, 600), "WebWork Game");
    window.setKeyRepeatEnabled(true);
    window.requestFocus();
    
    // 載入字型和圖片，處理可能的錯誤
    if (!font.loadFromFile("/System/Library/Fonts/STHeiti Light.ttc")) {
        cout << "Warning: Could not load primary font, trying backup font...\n";
    }
    if (!questionTexture.loadFromFile("sources/question.png")) {
        cout << "Warning: Could not load question image. Using text instead.\n";
    }
    
    // 設定題目圖片大小和位置
    questionSprite.setTexture(questionTexture);
    float scale = min(
        800.0f / questionTexture.getSize().x,
        300.0f / questionTexture.getSize().y
    );
    questionSprite.setScale(scale, scale);
    questionSprite.setPosition(
        (800 - questionSprite.getGlobalBounds().width) / 2, 130
    );
}

// 顯示遊戲規則
void Typing::printGameRules() {
    cout << "\n=== WebWork Game Rules ===\n";
    cout << "你忘記要寫 webwork ！快截止了\n";
    cout << "在時間到以前輸入此方程式！\n";
    cout << "注意: 答案要完全符合題目的方程式\n";
    cout << "例如: 如果答案是2x，打 2*x 會是錯的\n\n";
}

// 檢查並使用女友幫助道具
void Typing::checkAndUseGirlfriendHelp() {
    if (player.getItemCount("girlfriendsHelp") > 0) {
        
        if (player.useItem("girlfriendsHelp")) {
            usingItem = true;
            cout << "前半的答案是： " << correctAnswer.substr(0, correctAnswer.length()/2) << "\n";
            cout << "Press Enter to continue...\n";
            cin.get();
        }
    }
}

// 處理學生突然發問的事件
void Typing::handleStudentAttack() {
    if (enemies[2]->getLevel() >= 1) { // Student is enemies[2]
        enemies[2]->attack(&player);
        isTimeFrozen = true;
        freezeTimer.restart();
    }
}

// 格式化時間顯示
string Typing::formatTime(const sf::Time& timeLeft) {
    int hours = 23;
    int minutes = 59;
    int seconds = 60 - timeLeft.asSeconds();
    
    if (seconds >= 60) {
        minutes += seconds / 60;
        seconds %= 60;
    }
    if (minutes >= 60) {
        hours += minutes / 60;
        minutes %= 60;
    }

    stringstream ss;
    ss << setfill('0') << setw(2) << hours << ":"
       << setfill('0') << setw(2) << minutes << ":"
       << setfill('0') << setw(2) << seconds;
    return ss.str();
}

// 更新遊戲結果狀態
void Typing::updateStatus(bool win) {
    if (win) {
        player.updateKnowledge(30);
        enemies[1]->levelUp();
        cout << "你完成了 WebWork!\n你的 knowledge 增加了 40 !\n";
    } else {
        player.updateKnowledge(-20);
        enemies[0]->levelUp();
        cout << "WebWork 截止時間到了...\n你的 knowledge 下降了 20 !\n";
        cout << "deadline 等級提升了!\n";
    }
}

// 啟動並運行遊戲主循環
bool Typing::start() {
    printGameRules();
    cout << "Press Enter to continue...\n";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();

    checkAndUseGirlfriendHelp();

    // 設置開始遊戲的提示文字
    sf::Text instructions;
    instructions.setFont(font);
    instructions.setCharacterSize(20);
    instructions.setFillColor(sf::Color::White);
    instructions.setString("Press Enter to start typing.");
    instructions.setPosition((800 - instructions.getGlobalBounds().width) / 2, 200);

    // 初始化遊戲狀態變數
    bool gameStarted = false;
    bool gameover = false;
    bool win = false;
    bool studentAttackTriggered = false;
    string userInput = "";

    // 如果使用了女友幫助，預先填入一半答案
    if (usingItem) {
        userInput = correctAnswer.substr(0, correctAnswer.length()/2);
    }

    // 初始化計時器
    sf::Clock gameClock;
    sf::Time gameStartTime;
    sf::Time endTime;
    sf::Time frozenTime = sf::Time::Zero;

    // 遊戲主循環
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && !gameStarted) {
                    gameStarted = true;
                    gameStartTime = gameClock.getElapsedTime();
                    endTime = gameStartTime + sf::seconds(40);
                }
            }
            else if (event.type == sf::Event::TextEntered && gameStarted && !gameover && !isTimeFrozen) {
                if (event.text.unicode == '\b' && !userInput.empty()) {
                    userInput.pop_back();
                }
                else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    userInput += static_cast<char>(event.text.unicode);
                }
            }
        }

        // 繪製遊戲畫面
        window.clear(sf::Color::Black);

        if (!gameStarted) {
            window.draw(instructions);
        }
        else {
            // 顯示題目和玩家輸入
            if (questionTexture.getSize().x > 0) {
                window.draw(questionSprite);
            }

            sf::Text inputText;
            inputText.setFont(font);
            inputText.setCharacterSize(20);
            inputText.setFillColor(sf::Color::White);
            inputText.setString("Your answer: " + userInput);
            inputText.setPosition(50, 450);
            window.draw(inputText);

            // 計算剩餘時間
            sf::Time currentTime = gameClock.getElapsedTime();
            sf::Time adjustedTime = currentTime;
            
            if (isTimeFrozen) {
                if (freezeTimer.getElapsedTime().asSeconds() >= 5.0f) {
                    isTimeFrozen = false;
                    frozenTime += sf::seconds(5.0f);
                }
            }
            
            adjustedTime -= frozenTime;
            sf::Time timeLeft = endTime - adjustedTime;

            // 觸發學生攻擊事件
            if (!studentAttackTriggered && gameStarted &&
                timeLeft.asSeconds() <= 20.0f && timeLeft.asSeconds() > 19.0f) {
                studentAttackTriggered = true;
                handleStudentAttack();
            }

            // 檢查遊戲是否結束
            if (timeLeft <= sf::Time::Zero || userInput == correctAnswer) {
                window.clear(sf::Color::Black);
                if (questionTexture.getSize().x > 0) {
                    window.draw(questionSprite);
                }
                
                // 顯示最終結果
                sf::Text inputText;
                inputText.setFont(font);
                inputText.setCharacterSize(20);
                inputText.setFillColor(sf::Color::White);
                inputText.setString("Your answer: " + userInput);
                inputText.setPosition(50, 450);
                window.draw(inputText);

                sf::Text resultText;
                resultText.setFont(font);
                resultText.setCharacterSize(30);
                resultText.setPosition(80, 300);
                
                if (userInput == correctAnswer) {
                    resultText.setString("Great!\nYou completed the homework before deadline!");
                    resultText.setFillColor(sf::Color::Green);
                    win = true;
                } else {
                    resultText.setString("Time's up!\nThe deadline has passed!\nYou lost some homework grade.");
                    resultText.setFillColor(sf::Color::Red);
                }
                window.draw(resultText);
                window.display();
                
                sf::sleep(sf::seconds(2));
                window.close();
                return win;
            }
            else {
                // 顯示剩餘時間
                sf::Text timeText;
                timeText.setFont(font);
                timeText.setCharacterSize(24);
                timeText.setFillColor(sf::Color::White);
                if (isTimeFrozen) {
                    timeText.setString("Suddenly, your stupid student asked you a stupid question\n\n Time until deadline: " + formatTime(timeLeft) + " (FROZEN)");
                    timeText.setFillColor(sf::Color::Yellow);
                } else {
                    timeText.setString("Time until deadline: " + formatTime(timeLeft));
                }
                timeText.setPosition(100, 30);
                window.draw(timeText);
            }
        }

        window.display();
    }
    return false;
}
