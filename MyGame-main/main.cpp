#include "Game.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

int main(){
    string name;
    cout << "Enter your name and start\n";
    cin >> name;

    Game game(name);

    game.start();

    return 0;
}
