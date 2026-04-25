#include "Tutor.h"
#include <string>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <limits>

Tutor::Tutor(Player& player, vector<Enemy*>& enemies) // 傳遞參數給父類別建構子
    : MiniGame(player, enemies) {
}
void Tutor::updateStatus(bool win) {
        //贏了輸了會怎樣
        if(win){
            player.updateKnowledge(30) ;
            player.updateFriendship(20);
            player.addItem("salary");

            enemies[1]->levelUp();

            cout << "恭喜你贏得了這場遊戲！\n";
            cout << "你的 knowledge 增加了 30 !\n";
            cout << "你的 friendship 增加了 20 !\n";
            cout << "liver 等級提升了\n";

        }
        else{
            cout << "你輸了這場遊戲！\n";
            
            enemies[2]->levelUp();
            enemies[3]->levelUp();
            enemies[3]->levelUp();

            cout << "Student 等級提升了\n";
            cout << "EMO 等級提升了 2\n";
            
        }
}


bool Tutor::start() {
    bool win = true;
    int winThreshold = 40; // 勝利條件：敲擊次數
    int doubleHammer = 2;  // 道具：雙倍錘子

    // 劇情輸出
    cout << "這是你今天去試教的新家教學生，學生很笨，你覺得很躁，所以你要打爆家教學生的腦子！\n";
    cout << "在這一關裡，你要在 10 秒內連續敲擊「一次空白鍵加一次enter鍵的組合」，如果可以連續敲擊超過 " << winThreshold << " 組，就贏了！\n";
    cout << endl ;
    cout << "      _________       \n";
    cout << "   .-'         `-.    \n";
    cout << "  /             \\   \n";
    cout << " /               \\  \n";
    cout << "|   O       O     |  \n";
    cout << "|     _____       |  \n";
    cout << " \\    \\___/      /   \n";
    cout << "  \\             /    \n";
    cout << "   `-._______.-'     \n";
    cout << endl ;


     
    bool useDoubleHammer = true;  //如果有錘子，會在 Player 的 useItem 裡面問玩家是否要用
    
    if(player.getItemCount("hammer") == 0)
        useDoubleHammer = false;
    
    cin.ignore();
    if(!player.useItem("hammer") && player.getItemCount("hammer") != 0)
    {
        useDoubleHammer = false ;
        cout << "居然不用! 好吧那你加油\n" ;
    }
    
        
    
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // 遊戲開始倒數
    cout << "準備開始遊戲！請按 Enter 開始";
    cin.get(); // 等待玩家按下 Enter
    cout << "開始！" << endl;
    

    // 倒計時 10 秒
    auto startTime = chrono::high_resolution_clock::now();
    auto endTime = startTime + chrono::seconds(10);

    int keyPressCount = 0;

    // 計算空白鍵敲擊次數
    while (chrono::high_resolution_clock::now() < endTime) {
        if (cin.peek() != EOF) { // 如果有輸入
            char ch;
            cin.get(ch); // 讀取輸入字元
            if (ch == ' ') { // 如果是空白鍵
                keyPressCount += useDoubleHammer? doubleHammer : 1; // 如果用了雙倍錘子，敲擊次數加倍
            }
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // 遊戲結束
    cout << "遊戲結束！\n";
    cout << "你在 10 秒內敲擊了 " << keyPressCount << " 次空白鍵！\n";

    // 判斷遊戲結果
    win = keyPressCount >= winThreshold;
    
    if (win) {
        cout << "你贏得了這場遊戲！\n";
        cout << "你爆擊了學生，學生等級下降了！\n";
        player.attack(enemies[2]);
    }
    // 更新玩家和遊戲的狀態
    return win;
}
