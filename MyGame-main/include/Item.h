#ifndef ITEM_H
#define ITEM_H
#include <string>

using std::string;
using std::vector;

using namespace std;

class Player; // 前向宣告

class Item {
protected:
    string name;
    
public:
    Item(const string& n) : name(n) {}
    virtual ~Item() = default;
    
    string getName() const { return name; }
    virtual void use(Player& player) = 0;
    virtual void itemInfo() = 0;
};

class GirlFriendsHelp : public Item {
public:
    GirlFriendsHelp() : Item("girlfriendshelp") {}
    void use(Player& player) override;
    void itemInfo() override;
};

class GoodPersonCard : public Item {
public:
    GoodPersonCard() : Item("goodPersonCard") {}
    void use(Player& player) override;
    void itemInfo() override;
};

class Hammer : public Item {
public:
    Hammer() : Item("hammer") {}
    void use(Player& player) override;
    void itemInfo() override;
};

class Salary : public Item {
public:
    Salary() : Item("salary") {}
    void use(Player& player) override;
    void itemInfo() override;
};

#endif
