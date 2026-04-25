#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#include "Item.h"
#include <map>
#include <string>

using namespace std;

class Player : public Character {
private:
    int knowledge;
    int friendship;
    int romance;
    
    map<string, int> items;      // 數量
    map<string, Item*> itemTypes; // name, ItemPtr

public:
    Player();
    ~Player();
    
    int getKnowledge() const { return knowledge; }
    int getFriendship() const { return friendship; }
    int getRomance() const { return romance; }
    
    void setName(const string& n) { this->name = n; }
    
    void updateKnowledge(int val) { knowledge += val; }
    void updateFriendship(int val) { friendship += val; }
    void updateRomance(int val) { romance += val; }
    void updateLevel();
        
    int getItemCount(const string& itemName) const; // 獲得特定道具數量
    void addItem(const string& itemName);  // 增加特定道具數量
    bool useItem(const string& itemName);  // 使用道具
        
    void attack(Character* target) override;
};

#endif
