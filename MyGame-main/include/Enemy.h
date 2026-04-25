#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"
#include <iostream>

using namespace std;

class Enemy : public Character {
private:

public:
    Enemy() : Character() {};
    Enemy(const string& n) : Character(n) {};
    void setName(const string& n) { this->name = n; };
    virtual void attack(Character* target ) = 0;
    virtual ~Enemy() = default;
};


class Liver : public Enemy {
private:

public:
    Liver() : Enemy("liver") {};
    Liver(const string& n) : Enemy(n) {};
    void attack(Character* target) override;
};

class Emo : public Enemy {
private:

public:
    Emo() : Enemy("emo") {};
    Emo(const string& n) : Enemy(n) {};
    void attack(Character* target) override;
};

class Student : public Enemy {
private:

public:
    Student() : Enemy("student") {};
    Student(const string& n) : Enemy(n) {};
    void attack(Character* target) override;
};

class Deadline : public Enemy {
private:

public:
    Deadline() : Enemy("deadline") {};
    Deadline(const string& n) : Enemy(n) {};
    void attack(Character* target) override;
};

#endif
