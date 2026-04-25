#ifndef CHARACTER_H
#define CHARACTER_H
#include <string>

class Character {
protected :
    std::string name ;
    int level ;
public :
    Character(): name(""), level(0) {};
    Character(const std::string& n) : name(n), level(0) {};
    std::string getName() const { return name; };
    int getLevel() const { return level; };
    void levelUp() { level++; };
    void levelDown(){ level--; };
    virtual void attack (Character* target) = 0;
};

#endif
