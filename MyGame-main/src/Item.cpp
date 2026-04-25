#include "Item.h"
#include "Player.h"
#include <iostream>
using namespace std;

// 隨便亂寫的，只是要試跑
void GirlFriendsHelp::use(Player& player) {
    cout << "[道具使用] \n" ;
    cout << "你可愛的女朋友真是幫了你一個大忙\n" ;
    cout << "他怎麼知道因為 ^ 會讓你覺得WebWork在嘲諷你^^ 所以你最不喜歡打x^2 \n" ;
    cout << "他幫你打了x^2 真是太貼心了\n" ;
}

void GoodPersonCard::use(Player& player) {
    cout << "[道具使用] \n" ;
    cout << "俗話說交朋友就是要先從幫個忙開始\n" ;
    cout << "雖然不知道你會叫他會幫什麼忙 但他決定先跟你交個朋友\n" ;
    cout << "真的是個貨真價實的好人呢 ! \n" ;
    cout << "他讓你的 friendship 上升了 25 ! 耶呼! \n" ;
    cout << endl ;
    player.updateFriendship(25) ;
    
}

void Hammer::use(Player& player) {
    cout << "[道具使用] \n" ;
    cout << "你現在打一下會變兩下 猛得跟鬼一樣 \n" ;
}

void Salary::use(Player& player) {
    cout << "[道具使用] \n" ;
    cout << "你用 salary 660元 課了 ChatGPT Plus \n";
    cout << "他告訴你這題答案是：break";
}

void Salary::itemInfo() {
    cout << "你現在有薪水 660元，是否要當個 AI 盤子？用錢買學分？\n";
}

void Hammer::itemInfo() {
    cout << "這是一隻星爆氣流錘，可以用雙倍速度錘爆笨學生 ! \n";
}

void GoodPersonCard::itemInfo() {
    cout << "你可以用好人卡召喚出一個好人！\n" ;
}

void GirlFriendsHelp::itemInfo() {
    cout << "你的女朋友曾經答應要幫你一個忙，他現在可以幫你寫一半功課\n" ;
}
