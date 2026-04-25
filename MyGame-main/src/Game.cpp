#include "Game.h"
#include "Alcohol.h"
#include "CardMatch.h"
#include "Typing.h"
#include "Debug.h"
#include "Tutor.h"
#include "Dating.h"
#include <filesystem>
using namespace std;

Game::Game(const string& playerName) {
    succeed = false;
    player.setName(playerName);
    initEnemies();
}

void Game::initEnemies() {
    enemies.push_back(new Deadline);
    enemies.push_back(new Liver);
    enemies.push_back(new Student);
    enemies.push_back(new Emo);
}

bool Game::isValidInput(const string& inputStr) {
    if (inputStr.empty()) return false;
    for(int i = 0 ; i < inputStr.size() ; i++) {
        if(!isdigit(inputStr[i]))
            return false;
    }
    int value = stoi(inputStr);
    if (value == 0) return true;
    return value >= MIN_GAME_CHOICE && value <= MAX_GAME_CHOICE;
}

void Game::start() {
    // 玩家選擇
    while (1) {
        if (canGraduate()) { // 可以畢業了
            succeed = true;
            break;
        }

        printHomePage();
        
        cout << "Choose next game\n";
      
        string gameChoose;
        cin >> gameChoose;
        
        while (!isValidInput(gameChoose)) {
            cout << "就叫你從 1~6 選齁，不想玩就選 0 休學啦！\n";
            cin >> gameChoose;
        }
        
        if (gameChoose == "0") break;
        
        startGame(stoi(gameChoose));
        player.updateLevel();
    }
        
    if (succeed) {
        cout << "\n耶耶耶恭喜" << player.getName() << "畢業快樂！\n啦啦啦～～～\n";
        cout << "\n";
        cout << " _    _                            _____               _             _   _\n";
        cout << "| |  | |                          / ____|             | |           | | (_)\n";
        cout << "| |__| | __ _ _ __  _ __  _   _  | |  __ _ __ __ _  __| |_   _  __ _| |_ _  ___  _ __ \n";
        cout << "|  __  |/ _` | '_ \\| '_ \\| | | | | | |_ | '__/ _` |/ _` | | | |/ _` | __| |/ _ \\| '_ \\ \n";
        cout << "| |  | | (_| | |_) | |_) | |_| | | |__| | | | (_| | (_| | |_| | (_| | |_| | (_) | | | |\n";
        cout << "|_|  |_|\\__,_| .__/| .__/ \\__, |  \\_____|_|  \\__,_|\\__,_|\\__,_|\\__,_|\\__|_|\\___/|_| |_|\n";
        cout << "             | |   | |     __/ |\n";
        cout << "             |_|   |_|    |___/\n";
        cout << "\n";
                                                                                       
        exit(0);
    } else {
        cout << "你休學了:( \n";
        exit(0);
    }
}

bool Game::canGraduate() const{
    if (player.getKnowledge() >= GRADUATE_KNOWLEDGE
        && player.getFriendship() >= GRADUATE_FRIENDSHIP
        && player.getRomance() >= GRADUATE_ROMANCE)
        return true;
    else return false;
}

bool Game::startGame(int gameChoose) {
        
    // 設定正確的路徑
    auto currentPath = filesystem::current_path();
    auto projectPath = filesystem::path("/Users/jianlipei/Documents/MyGame");
    filesystem::current_path(projectPath);
    
    MiniGame* nowGame;
    bool win = false;
    
    try {
        switch(gameChoose) {
            case 1:
                nowGame = new Alcohol(player, enemies);
                break;
            case 2:
                nowGame = new CardMatch(player, enemies);
                break;
            case 3:
                nowGame = new Dating(player, enemies);
                break;
            case 4:
                nowGame = new Debug(player, enemies);
                break;
            case 5:
                nowGame = new Tutor(player, enemies);
                break;
            case 6:
                nowGame = new Typing(player, enemies);
                break;
            default:
                return false;
        }
        if (nowGame) {
            win = nowGame->start();
            nowGame->updateStatus(win);
            delete nowGame;
        }
        filesystem::current_path(currentPath);
        return win;
    } catch (const exception& e) {
        cerr << "遊戲執行發生錯誤: " << e.what() << endl;
        filesystem::current_path(currentPath);
        return false;
    }
}
    
void Game::printHomePage() {
    // 製作畫面
    cout << "\nWelcome!\n";
    cout << "Press Enter to choose game\n" ;
    cin.ignore() ;
    cin.ignore() ;
    
    // cin 1~6: playgame, 0: quit game
    cout << "+--------------+  +--------------+  +--------------+\n" ;
    cout << "|    Game 1    |  |    Game 2    |  |    Game 3    |\n" ;
    cout << "|--------------|  |--------------|  |--------------|\n" ;
    cout << "|    Drink     |  |    Who is    |  | Your cuteeee |\n" ;
    cout << "|   alcohol!   |  |  your friend |  |  girlfriend  |\n" ;
    cout << "+--------------+  +--------------+  +--------------+\n" ;
    cout << endl ;
    cout << "+--------------+  +--------------+  +--------------+\n" ;
    cout << "|    Game 4    |  |    Game 5    |  |    Game 6    |\n" ;
    cout << "|--------------|  |--------------|  |--------------|\n" ;
    cout << "|   Time to    |  |   Annoying   |  |     OMG      |\n" ;
      cout << "|  debuggggg!  |  |   student    |  |   WebWork    |\n" ;
    cout << "+--------------+  +--------------+  +--------------+\n" ;
    cout << endl ;
    
    cout << "Your points now:\n";
    cout << "knowledge: " << player.getKnowledge()
         << ", friendship: " << player.getFriendship()
         << ", romance: " << player.getRomance() << '\n';
         
    int knowledgeToGraduate = (GRADUATE_KNOWLEDGE - player.getKnowledge() > 0)? GRADUATE_KNOWLEDGE - player.getKnowledge() : 0;
    int friendshipToGraduate = (GRADUATE_FRIENDSHIP - player.getFriendship())? GRADUATE_FRIENDSHIP - player.getFriendship() : 0;
    int romanceToGraduate = (GRADUATE_ROMANCE - player.getRomance())? GRADUATE_ROMANCE - player.getRomance() : 0;
    cout << "You still need " << knowledgeToGraduate << " knowledge, "
         << friendshipToGraduate << " friendship, "
         << romanceToGraduate << " romance to graduate.\n"
         << "\nYour level is: " << player.getLevel() << '\n';
    
    cout << "\nYour items:\n";
    cout << "hammer: " << player.getItemCount("hammer")
         << ", goodPersonCard: " << player.getItemCount("goodPersonCard")
         << ", girlfriendsHelp: " << player.getItemCount("girlfriendsHelp")
         << ", salary: " << player.getItemCount("salary") << '\n';

    cout << "\nEnemy levels:\n";
    for(const auto& enemy : enemies) {
        cout << enemy->getName() << ": Level " << enemy->getLevel() << "; ";
    }
    cout << "\n\n如果想休學，請輸入0\n";
    cout << '\n' << '\n';
    
}
