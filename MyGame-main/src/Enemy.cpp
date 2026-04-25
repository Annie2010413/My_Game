#include "Enemy.h"
#include "Player.h"
using namespace std;

void Deadline::attack(Character* target) {
    cout << "\n死線太接近了\n" ;
    cout << "怎麼辦怎麼辦 你不知道該怎麼辦 \n" ;
    cout << "你因為緊張焦慮到沒辦法做事了QAQ \n" ;
    cout << "你的 knowledge 下降了 5 \n" ;

    Player* playerTarget = dynamic_cast<Player*>(target);
    playerTarget->updateKnowledge(-5);
}

void Liver::attack(Character* target) {
    cout << "你的肝操勞指數過高 然後你還不休息\n" ;
    cout << "所以肝生氣了! 他決定烙他的器官兄弟讓你頭痛欲裂\n" ;
    cout << "因為頭痛欲裂所以你必須中途離開\n" ;
    cout << ": 掰掰! 早日康復哦\n" ;
    cout << endl ;
    cout << "喔對 因為你離開的時候忘記跟你女朋友說了 所以他有點小難過\n" ;
    cout << "你的 romance 下降了 5 \n" ;
    
    Player* playerTarget = dynamic_cast<Player*>(target);
    playerTarget->updateRomance(-5) ;
}


void Student::attack(Character* target) {
    cout << "\n家教笨學生突然問你問題！！！\n";
    cout << "你為了趕在死線前寫完 Webwork 敷衍地叫他等等\n";
    cout << "家教學生因為覺得被你塑膠了而不太開心\n";
    cout << "你的 friendship 下降了 5 \n";
    
    Player* playerTarget = dynamic_cast<Player*>(target);
    playerTarget->updateFriendship(-5);
}

void Emo::attack(Character* target) {
    cout << "\n你因太悲傷了，眼前一片模糊，什麼都看不太清楚\n";
}
