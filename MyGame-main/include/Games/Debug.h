#ifndef DEBUG_H
#define DEBUG_H
#include "../MiniGame.h"

class Debug : public MiniGame {
private:
    struct Question {
        string description;       // 題目敘述
        string codeSnippet;      // 程式碼片段
        string blankPlaceholder; // 空格佔位符，例如 "____"
        string correctAnswer;    // 正確答案
    };
    
    vector<Question> questions;
    int currentQuestionIndex;
    int timeLimitSeconds;
    
    // 處理限時輸入
    mutex mtx;
    condition_variable cv;
    bool inputReceived;
    string playerAnswer;

    void loadQuestions();          // 載入題目
    string getPlayerInput();       // 獲取玩家輸入

    // 移除字串中的所有空白字符
    static string removeSpaces(const string& str) {
        string result;
        for(char c : str){
            if(!isspace(static_cast<unsigned char>(c))){
                result += c;
            }
        }
        return result;
    }

public:
    Debug(Player& player, vector<Enemy*>& enemies);
    void updateStatus(bool win) override;
    bool start() override;
    
};
#endif
