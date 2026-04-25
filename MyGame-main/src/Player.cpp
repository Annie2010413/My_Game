#include "Player.h"
#include "Enemy.h"
#include <iostream>

Player::Player() : Character() {
    knowledge = 0;
    friendship = 0;
    romance = 0;
    level = 0;
    
    items["hammer"] = 0;
    items["goodPersonCard"] = 0;
    items["girlfriendsHelp"] = 0;
    items["salary"] = 0;

    itemTypes["hammer"] = new Hammer();
    itemTypes["goodPersonCard"] = new GoodPersonCard();
    itemTypes["girlfriendsHelp"] = new GirlFriendsHelp();
    itemTypes["salary"] = new Salary();
}

Player::~Player() {
    for(auto& item : itemTypes) {
        delete item.second;
    }
    itemTypes.clear();
}

void Player::updateLevel() {
    if (level == 2 && knowledge >= 100 && friendship >= 100 && romance >= 100)
        this->levelUp();
    else if (level == 1 && knowledge >= 50 && friendship >= 50 && romance >= 50)
        this->levelUp();
    else if (level == 0 && knowledge >= 30 && friendship >= 30 && romance >= 30)
        this->levelUp();
}

int Player::getItemCount(const string& itemName) const {
    auto it = items.find(itemName);
    if (it != items.end()) {
        return it->second;
    }
    return 0;
}

void Player::addItem(const string& itemName) {
    items[itemName]++;
    cout << "\n[道具獲得]\n" ;
    cout << "哇哇哇哇 " << name << " 拿到了 " << itemName << " !" <<endl ;
}


bool Player::useItem(const string& itemName) {
    if (items[itemName] > 0) {
        cout << "還記得你曾經拿到的 " << itemName << " 嗎? \n" ;
        cout << "現在是他們派上用場的時候啦 ! \n" ;
        itemTypes[itemName]->itemInfo();
        cout << "你想要使用道具 " << itemName << " 嗎? (y / n) : " ;
        
        string choice;
        while (true) {
            getline(cin, choice);
            
            if (choice == "y" || choice == "Y") {
                cout << endl;
                itemTypes[itemName]->use(*this);
                items[itemName]--;
                return true;
            }
            if (choice == "n" || choice == "N") {
                cout << endl;
                return false;
            }
            
            cout << "不要亂輸入! 請輸入 y 或 n : ";
        }
    }
    return false;

}

void Player::attack(Character* target) {
    Enemy* enemyTarget = dynamic_cast<Enemy*>(target);
    enemyTarget->levelDown();
}
