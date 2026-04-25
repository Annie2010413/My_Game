#include "Alcohol.h"
#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <vector>

string Alcohol::pssGetChoiceName(int choice){
    switch (choice) {
        case 1: return "剪刀";
        case 2: return "石頭";
        case 3: return "布";
        default: return "";
    }
}
int Alcohol::pssDetermineWinner(int userChoice, int computerChoice){
    if (userChoice == computerChoice) {
        cout << "_> 平手! 再來一次！\n";
        cout << endl ;
        cout << "------------------------------\n" ;
        cout << endl ;
        return 2 ;
    } else if ((userChoice == 1 && computerChoice == 3) || // 剪刀贏布
               (userChoice == 2 && computerChoice == 1) || // 石頭贏剪刀
               (userChoice == 3 && computerChoice == 2)) { // 布贏石頭
        cout << "_> 你贏了欸!\n";
        return 1 ;
    } else {
        cout << "_> 你輸了哦!\n";
        return 0 ;
    }
}

bool Alcohol::paperScissorStone(){
   while(1){
        srand(static_cast<int>(time(0))); // 以時間設定隨機數 seed

        string inputStr ;
        bool wrongInput = false ;
        int userChoice  ;
        cout << "你要出什麼拳 (請輸入數字 1~3)\n";
        cout << "+---------------+ \n" ;
        cout << "|  1 : scissor  | \n" ;
        cout << "|  2 : stone    | \n" ;
        cout << "|  3 : paper    | \n" ;
        cout << "+---------------+ \n" ;
        cout << "我想出: ";
        cin >> inputStr;
        cout << endl ;
        for(int i = 0 ; i < inputStr.size() ; i++)
        {
            if(!isdigit(inputStr[i])){
                cout << "不要亂輸入! 你醜一哦!! 1~3選一個!\n";
                cout << endl ;
                wrongInput = true ;
                break ;
            }
            
        }
        if(!wrongInput)
            userChoice = stoi(inputStr) ;
        else
            continue;

        // 檢查輸入
        if (userChoice < 1  || userChoice > 3) {
            cout << "不要亂輸入! 你醜一哦!! 1~3選一個!\n";
            cout << endl ;
            continue;
        }

        int computerChoice = rand() % 3 + 1; // 電腦隨機選

        // 顯示結果
        cout << "你出了: " << pssGetChoiceName(userChoice) << endl;
        cout << "你朋友出了: " << pssGetChoiceName(computerChoice) << endl;
        cout << endl ;

        // 判斷結果
        int pssWin = pssDetermineWinner(userChoice, computerChoice) ;
        
        if(pssWin == 1)
            return true ;
        if(pssWin == 0)
            return false ;
    }
    
}

string Alcohol::alcGetChoiceName(int choice){
    switch (choice) {
        case 1: return "伏特加";
        case 2: return "柳橙汁";
        case 3: return "百威啤酒";
        default: return "";
    }
}

int Alcohol::chooseAlcohol(int drunkLevel){
    srand(static_cast<int>(time(0)));

    while(1){
        cout << "+---- 選一杯叭！(請輸入數字 1~3) ----+\n" ;
        cout << "|                                    |\n" ;
        cout << "|   \\    /    \\    /    \\    /       |\n" ;
        cout << "|    \\ _/      \\ _/      \\ _/        |\n" ;
        cout << "|     1         2         3          |\n" ;
        cout << "+------------------------------------+\n" ;
        cout << "我想喝: " ;
        
        string inputStr ;
        bool wrongInput = false ;
        int cupNum ;
        cin >> inputStr;
        cout << endl ;
        for(int i = 0 ; i < inputStr.size() ; i++)
        {
            if(!isdigit(inputStr[i])){
                cout << "不要亂喝! 就叫你選 1~3!! 你醉了嗎 \n" ;
                cout << endl ;
                wrongInput = true ;
                break ;
            }
            
        }
        if(!wrongInput)
            cupNum = stoi(inputStr) ;
        else
            continue;
        

        // 檢查輸入
        if(cupNum < 1 || cupNum > 3)
        {
            cout << "不要亂喝! 就叫你選 1~3!! 你醉了嗎 \n" ;
            cout << endl ;
            continue ;
        }

        int yourChoice = ((rand() % 1000) * cupNum) % 3 + 1;

        // 有人幫擋酒
        cin.ignore();
        if(player.useItem("goodPersonCard")){
            cout << "你拜託了好人幫你擋酒\n" ;
            cout << "他幫你喝了 \\" << alcGetChoiceName(yourChoice) << "/ !" << endl;
            cout << "真是個好人 :) \n" ;
            return drunkLevel ;
        }

        cout << "你選到 \\" << alcGetChoiceName(yourChoice) << "/ 了！" << endl;

        if(yourChoice == 1) { // 選到伏特加
            drunkLevel += 50 ;
            cout << "伏特加讓你的喉嚨快燒起來了:( \n" ;
            cout << "你的喝醉指數增加了 50\n" ;
        }
        else if(yourChoice == 2){ // 選到柳橙汁
            if(drunkLevel == 0){
                cout << "太幸運了吧 哪有人第一杯就先喝柳橙汁! \n" ;
            }
            else if(drunkLevel > 10){
                drunkLevel -= 10 ;
                cout << "柳橙汁幫你解酒! 你的喝醉指數下降了 10\n" ;
            }
            else{
                drunkLevel = 0;
                cout << "柳橙汁幫你解酒! 你整個人都醒了\n" ;
            }
        }
        else{ // 選到百威
            drunkLevel += 25 ;
            cout << "百威讓你全身都是啤酒味! 你的喝醉指數增加了25\n" ;
        }

        return drunkLevel ;
    }
}

void Alcohol::gotHammar(){
    cout << "啊!在喝完酒後突然想到還要回管圖拿書包欸! \n" ;
    cout << "身為一顆卷心菜怎麼能忍受自己一天不卷呢 \n" ;
    cout << "把書包丟在管圖就回家是絕對不可能的\n" ;
    cout << "( 出發前往管圖 ) \n" ;
    cout << "( 到管圖了 ! ) \n" ;
    cout << "感覺現在好像有點茫 ~ _ ~ \n" ;
    cout << "哇! 角落有一支很可愛的槌子欸~~~~ \n" ;
    cout << "( 拿走槌子 ) \n" ;
    player.addItem("hammer") ;
}

void Alcohol::updateStatus(bool win) {
        if(win){
            player.updateRomance(50) ;
            cout << "你拚完酒沒醉，還記得要送你女朋友回家\n";
            cout << "你的女朋友非常開心! \n" ;
            cout << "你的 romance 上升了 " << 50 << endl ;
        }
        else{
            player.updateFriendship(-50) ;
            cout << "你喝醉吐在你朋友身上，他決定跟你絕交三天\n";
            cout << "你的 friendShip 下降了 " << 50 << endl ;
        }
}
bool Alcohol::start() {
    bool win = true ;

    // 一些劇情
    cout << "今天你跟女朋友一起去了一年一度的管院舞會。你被拱上台玩拚酒遊戲\n";
    cout << "你的目標是在三回合內不要喝到吐，每回合猜拳輸了就要喝一杯 ! \n";
    cout << "這是一場關乎面子的戰爭，請認真面對 !!!!\n";
    cout << endl ;
        
    
    int drunkLevel = 0 ;
    for(int i = 0 ; i < MAX_ROUND_CNT ; i++) { // 玩3次
        cout << "================ 第" << i+1 << "局 ================\n";
        cout << endl ;
        cout << "\\ 猜拳時間！/ \n" ;
        bool pssWin = paperScissorStone() ;
        if(!pssWin){
            cout << endl ;
            cout << "\\ 喝酒喝酒! /  \n" ;
            drunkLevel = chooseAlcohol(drunkLevel) ;
            cout << "你現在的喝醉指數是: " << drunkLevel << endl ;
            cout << endl ;
            if(drunkLevel > 75){
                win = false ;
                cout << "遊戲結果 : 你喝到吐了哈哈哈哈\n" ;
                break ;
            }
            if(player.getLevel() < enemies[1]->getLevel()) { //如果肝等級大於玩家就attack
                cout << "突發事件!\n" ;
                cout << "你正在被肝攻擊...\n" ;
                //cout << "你的肝就快壞了還在喝酒! \n" ;
                cout << endl ;
                enemies[1]->attack(&player) ;
                win = false ;
                return win ;
            }
        }
        else{
            cout << "原來你就是猜拳之神嗎！被你逃過一杯了 \n" ;
            cout << endl ;
        }
       
    }
    
    if(win)
        cout << "遊戲結果 : 你在3局拚酒裡面活下來了!\n" ;
    cout << endl ;
    
    if(win){
        // 額外事件 --> 撿到槌子
        cout << endl ;
        cout << "\n--------- Bonus 事件! ---------\n" ;
        cout << "Press Enter to see ! " ;
        cout << endl ;
        cin.ignore();
        cin.ignore();
        gotHammar() ;
    }
    
    return win;
}
