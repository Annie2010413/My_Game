#include "Dating.h"
#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <limits>

Dating::Dating(Player& player, vector<Enemy*>& enemies)
    : MiniGame(player, enemies), answer(""), answered(false) {}

// 等待玩家按下 Enter 鍵繼續
void Dating::pressEnterToContinue() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// 逐行顯示文本並等待玩家按 Enter 鍵繼續
void Dating::displayLines(const string lines[], int size) {
    for (int i = 0; i < size; i++) {
        size_t pos_right = lines[i].find("（置右）");
        if (pos_right != string::npos) {
            const int terminalWidth = 80;
            string message = lines[i].substr(0, pos_right);
            size_t msgLength = message.length();
            if (msgLength < static_cast<size_t>(terminalWidth)) {
                int padding = terminalWidth - static_cast<int>(msgLength);
                cout << string(padding, ' ') << message << endl;
            } else {
                cout << message << endl;
            }
        } else {
            cout << lines[i] << endl;
        }
        pressEnterToContinue();
    }
}

// 輸入處理函數
void Dating::getInput() {
    getline(cin, answer);
    answered = true;
}

// 處理單一題目
void Dating::handleQuestion(const string dialogues[], int dialogues_size, const string options[], const string correct_answer) {
    displayLines(dialogues, dialogues_size);

    cout << "\n以下哪家餐廳最可能是寶寶今天最想吃的？" << endl;
    for (int i = 0; i < 3; ++i) {
        cout << options[i] << endl;
    }

    const string valid_answers[] = { "A", "B", "C" };
    const int valid_size = 3;

    // 初始化回答狀態
    answered = false;
    answer = "";

    cout << "請輸入你的答案（A / B / C）：";

    // 啟動輸入執行緒
    thread input_thread(&Dating::getInput, this);

    // 等待輸入或者超時（5秒）
    const int timeout_seconds = 5;
    bool valid_input = false;
    for (int i = 0; i < timeout_seconds * 10; i++) { // 每100ms檢查一次，總共15秒
        if (answered) {
            // 將小寫轉為大寫
            for (size_t j = 0; j < answer.length(); ++j) {
                if (answer[j] >= 'a' && answer[j] <= 'z') {
                    answer[j] = answer[j] - 'a' + 'A';
                }
            }
            // 去除前後空白
            size_t start = answer.find_first_not_of(" \t");
            size_t end = answer.find_last_not_of(" \t");
            if (start != string::npos && end != string::npos) {
                answer = answer.substr(start, end - start + 1);
            } else {
                answer = "";
            }

            // 檢查是否為有效答案
            bool isValid = false;
            for (int k = 0; k < valid_size; ++k) {
                if (answer == valid_answers[k]) {
                    isValid = true;
                    break;
                }
            }

            if (isValid) {
                valid_input = true;
                break; // 有效答案，結束等待
            } else {
                cout << "無效的選擇，請輸入 A、B 或 C。" << endl;
                // 重置回答狀態以繼續等待
                answered = false;
                answer = "";
                cout << "請重新輸入你的答案（A / B / C）：";
                getInput();
            }
        }
        this_thread::sleep_for(chrono::milliseconds(100));
    }

    // 如果仍未回答，視為超時
    if (!valid_input) {
        cout << "\n時間到了！未能作答，答案視為錯誤！" << endl;
        answer = ""; // 設置為空字串，表示未回答
    }

    // 確保輸入執行緒結束
    if (input_thread.joinable()) {
        input_thread.detach(); // 分離輸入線程以避免阻塞
    }

    // 檢查答案是否正確
    cout << "\n" << string(50, '-') << "\n" << endl;
    if (answer != correct_answer) {
        cout << "答錯了！" << endl << "正確答案是 " << correct_answer << "！" << endl;
        cout << "寶寶：就知道你在敷衍我 ( 眼眶泛淚 ) " << endl;
    } else {
        cout << "寶寶：猜對的吧！算你厲害：（ " << endl;
    }

}


void Dating::updateStatus(bool win) {
    if (win) {
        enemies[1]->levelDown() ;
        enemies[3]->levelDown() ;
        cout << "你贏得了這場遊戲！" << endl;
        cout << "你的 romance 上升了 30\n" << endl;
        cout << "liver 等級下降了\n";
        cout << "EMO 等級下降了\n";
        player.updateRomance(30) ;
        cout << "\n--------- Bonus 事件! ---------\n" ;
        cout << "\n女朋友心情很好，決定答應你一件事！\n" ;
        gotGirlfriendHelp();
    }
    else {
        cout << "你輸了這場遊戲！" << endl;
        cout << "你的 romance 下降了 30\n" << endl;
        player.updateRomance(-30) ;
    }
}
void Dating::gotGirlfriendHelp(){
    cout << "寶寶：今天謝謝你陪我：） \n" ;
    cout << "：不用謝ㄚ！最愛尼了 \n" ;
    cout << "寶寶：webwork 寫完了嗎？不如我幫你一起寫吧！\n" ;
    player.addItem("girlfriendsHelp") ;
}

bool Dating::start() {
    bool win = true;
    int correct_answers = 0;

    // 前導劇情
    const int prelude_size = 6;
    const string prelude[prelude_size] = {
        "（你和你的女朋友走在椰林大道上散步）",
        "你：寶寶今天午餐要吃什麼鴨？",
        "寶寶：想吃好吃的！",
        "你：好...好吃的是吃什麼...？",
        "寶寶：我明明都跟你講過！！！（瞪）（大皺眉）（舉起拳頭）",
        "=== 糟糕訊息都沒認真看被抓到...女朋友要生氣了！===\n=== 快從對話紀錄推敲出你的女朋友今天想吃什麼！===\n=== 每題只有五秒的作答時間！準備好就開始吧！==="
    };
    displayLines(prelude, prelude_size);

    // 第一題
    const int dialogues1_size = 3;
    const string dialogues1[dialogues1_size] = {
        "第一題：",
        "寶寶早安!!好想你今天有睡飽\n飽八個小時沒見到你了好想你\n睡覺前想到你說想吃拉麵我也\n覺得天氣好冷想吃熱的但我不\n喜歡拉麵它好鹹好久沒吃火鍋\n了歐我們改天去吃火鍋好不好\n啊啊可是我也好想吃部隊鍋好\n多人我看到好多人的限動在發\n蛤討厭我們還是吃你想吃的好\n了我選不出來啦            11:37a.m.",
        "11:38a.m. 好啊寶寶說什麼都可以最愛尼（置右）"
    };
    const string options1[] = { "A.韓天閣", "B.唐老鴨", "C.墨洋拉麵" };
    handleQuestion(dialogues1, dialogues1_size, options1, "C");
    if (answer == "C") correct_answers++;
    cout << "\n" << string(50, '-') << "\n" << endl;

    // 第二題
    const int dialogues2_size = 3;
    const string dialogues2[dialogues2_size] = {
        "第二題：",
        "寶寶在嗎在嗎剛剛朋友傳了烤\n鬆餅的影片給我看起來好好吃\n喔我們去吃好不好蛤但晚餐吃\n鬆餅你會不會吃不飽還是改吃\n其他間對了不如去吃炸雞吧好\n胖好胖但好好吃雖然我覺得那\n家鴨血比炸雞好吃嘿嘿啊啊可\n是我好久沒吃義大利麵了耶想\n吃擺盤很漂亮的那種感覺可以\n吃一頓大的就當作慶祝情人節\n吧好耶那就吃這個然後我不在\n的時候也要乖乖的喔最愛你了\n等等見                    6:37p.m.",
        "6:38p.m. 當然可以啊寶寶最愛你了（置右）"
    };
    const string options2[] = { "A.塊雞師食務所", "B.小木屋鬆餅", "C.莫凡彼" };
    handleQuestion(dialogues2, dialogues2_size, options2, "C");
    if (answer == "C") correct_answers++;
    cout << "\n" << string(50, '-') << "\n" << endl;

    // 第三題
    const int dialogues3_size = 3;
    const string dialogues3[dialogues3_size] = {
        "第三題：",
        "寶寶程設作業寫完了嗎突然肚\n子好餓哦今天都沒有吃到甜的\n我需要一點糖分啊啊啊來去逛\n公館夜市好不好啊啊但感覺會\n很多人耶不要好了討厭討厭不\n喜歡排隊歐歐好像有點想吃冰\n你想吃嗎要吃雪花冰嗎歐耨可\n是那個很貴沒有錢嗚嗚嗚改吃\n粉圓冰好了好久沒吃但我每次\n都吃不完你要幫我吃哦嘻嘻嘻\n好喜歡你想去找你這樣又可以\n見面了一天見面三次真幸福啦\n啦啦啊不對但它今天好像沒有\n開耶好難過為什麼沒開這樣只\n能吃另一個冰了真是的好啦那\n個也不錯吃有吃冰就開心愛你\n耶耶耶                    11:37p.m.",
        "11:38p.m. 沒問題歐寶寶我也想見你嘻嘻（置右）"
    };
    const string options3[] = { "A.雪腐", "B.鴉片粉圓", "C.紫米牛奶" };
    handleQuestion(dialogues3, dialogues3_size, options3, "B");
    if (answer == "B") correct_answers++;

    if (correct_answers < 2)
        win = false;
        
    return win;
}
