#include "Character.h"
#include "Enemy.h"
#include "Debug.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <algorithm>
#include <limits>

Debug::Debug(Player& player, vector<Enemy*>& enemies)
    : MiniGame(player, enemies), currentQuestionIndex(0), inputReceived(false) { // 設定時間限制為30秒 // timeLimitSeconds(30)
    Character* enemy = enemies[0];
    timeLimitSeconds = enemy->getLevel() >= 1 ? 20 : 30;
    loadQuestions();
}

// 載入預設題目
void Debug::loadQuestions() {
    questions.push_back({
        "若程式的目標是要在 array 中找到第一個等於 key 的元素的 index，使得最後 F 輸出的結果為 2，____ 要填啥?",
        "int a[] = {0, 1, 5, 4, 5, 4, 5};\nint n = 7, i, key = 5, F = 0;\nfor (i = 0; i < n; i++) {\n    if (a[i] == key) {\n        F = i;\n        ____;\n    }\n}",
        "____",
        "break"
    });
    questions.push_back({
        "若要將陣列中 a[0] 的元素移到 a[n-1]，程式片段空白處該填入何運算式？",
        "int i, hold, n;\n// …\nfor (i = 0; i <=____ ; i = i + 1) {\n    hold = a[i];\n    a[i] = a[i + 1];\n    a[i + 1] = hold;\n}",
        "____",
        "n-2"
    });
}

// 更新玩家的狀態（答對或答錯）
void Debug::updateStatus(bool win) {
    if(win){
        cout << "恭喜你贏得了這場遊戲！\n";
        player.updateKnowledge(25) ;
        enemies[1]->levelUp() ;
        cout << "你的 knowledge 上升了 25\n" << endl;
        cout << "liver 等級上升了\n";
    }
    else{
        cout << "\n\n你輸了這場遊戲！\n";
        player.updateKnowledge(-25) ;
        enemies[3]->levelUp() ;
        cout << "你的 knowledge 下降了 25\n" << endl;
        cout << "EMO 等級上升了\n";
    }
}

// 獲取玩家輸入並處理超時z
string Debug::getPlayerInput() {
    // 重置輸入標記
    inputReceived = false;
    playerAnswer.clear();

    // 使用 unique_lock 來管理多線程同步
    unique_lock<mutex> lock(mtx);

    // 啟動新線程來讀取輸入
    thread inputThread([&]() {
        string threadInput;
        getline(cin, threadInput);
        {
            lock_guard<mutex> lg(mtx);
            playerAnswer = threadInput;
            inputReceived = true;
        }
        cv.notify_one();
    });

    // 等待直到收到輸入或者超過時間限制
    if (cv.wait_for(lock, chrono::seconds(timeLimitSeconds), [&]() { return inputReceived; })) {
        inputThread.join();
        return playerAnswer;
    }
    else {
        inputThread.detach();
        return "";
    }
}

// 開始迷你遊戲
bool Debug::start() {
    
    cout << "現在是星期二早上 7:59 分\n";
    cout << "還有一分鐘這週的程式作業就截止了！！\n";
    // 如果有被攻擊，要說，要用attack
    Character* enemy = enemies[0];
    // timeLimitSeconds = enemy->getLevel() >= 1 ? 20 : 30;
    if (enemy->getLevel() >= 1) cout << "但你剛剛 webwork 輸了，deadline level up，你現在剩下 40 秒答題\n";
    cout << "請你各花 " << timeLimitSeconds << " 秒回答下面兩題\n";
    
    cout << "=== Debug 迷你遊戲 ===\n";
    // cout << "請在每題 " << timeLimitSeconds << " 秒內填入程式碼中的空格。\n\n";
    
    bool allCorrect = true;
    
    for(currentQuestionIndex = 0; currentQuestionIndex < questions.size(); currentQuestionIndex++) {
        const Question& q = questions[currentQuestionIndex];
        
        cout << "題目 " << (currentQuestionIndex + 1) << ": " << q.description << "\n";
        cout << q.codeSnippet ;
        cout << endl ;
        
        if(currentQuestionIndex == 0)
        {
            cin.ignore();
            if(player.useItem("salary") && player.getItemCount("salary") != 0)
                cout << "居然不用! 好吧那你加油\n" ;
            cout << endl ;
        }
        
        cout << "請填入 " << q.blankPlaceholder << " 的答案：";
        
        //if(currentQuestionIndex == 0 && player.getItemCount("salary") == 0)
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string answer = getPlayerInput();
        
        if(answer.empty()) {
            cout << "\n時間到！未輸入答案。\n";
            allCorrect = false;
        }
        else {
            // 去除尾部空白
            answer.erase(answer.find_last_not_of(" \n\r\t")+1);
            
            // 比較時忽略所有空白字符
            if(removeSpaces(answer) == removeSpaces(q.correctAnswer)){
                cout << "\n答對了！\n\n";
            }
            else{
                cout << "\n答錯了！正確答案應為：" << q.correctAnswer << "\n";
                allCorrect = false;
            }
        }
    }
    
    bool win = allCorrect;
    return win;
}
