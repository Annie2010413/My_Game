#include "CardMatch.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <filesystem>

using namespace std;

// 建立一張新卡片，設定大小和載入圖片
Card::Card(int cardId, int mId, const string& frontImagePath)
    : id(cardId), matchId(mId), isRevealed(false), isMatched(false), effectShader(nullptr) {
    this->setSize(sf::Vector2f(100, 150));
    
    try {
        if (!frontTexture.loadFromFile(frontImagePath)) {
            throw runtime_error("無法載入正面圖片: " + frontImagePath);
        }
        
        if (!backTexture.loadFromFile("sources/card_back.jpg")) {
            throw runtime_error("無法載入背面圖片");
        }
        
        frontTexture.setSmooth(true);
        backTexture.setSmooth(true);
        this->setTexture(&backTexture);
        
    } catch (const exception& e) {
        cerr << "載入圖片錯誤：" << e.what() << endl;
        this->setFillColor(sf::Color(100, 100, 100));
    }
}

// 比較兩張卡片是否配對
bool Card::operator==(const Card& other) const {
    return this->matchId == other.matchId;
}

// 套用灰階效果
void Card::applyEffect(sf::Shader* shader) {
    sf::Color darkGray(100, 100, 100, 255);
    this->setFillColor(darkGray);
}

// 翻開卡片，顯示正面
void Card::reveal() {
    isRevealed = true;
    this->setTexture(&frontTexture);
    if (effectShader) {
        sf::Color darkGray(100, 100, 100, 255);
        this->setFillColor(darkGray);
    }
}

// 蓋住卡片，顯示背面
void Card::hide() {
    isRevealed = false;
    this->setTexture(&backTexture);
}

// 標記卡片已配對
void Card::match() {
    isMatched = true;
}

int Card::getId() const { return id; }
int Card::getMatchId() const { return matchId; }
bool Card::getIsRevealed() const { return isRevealed; }
bool Card::getIsMatched() const { return isMatched; }

void Card::setPosition(float x, float y) {
    sf::RectangleShape::setPosition(x, y);
}

CardMatch::CardMatch(Player& p, vector<Enemy*>& e)
    : MiniGame(p, e),
      window(sf::VideoMode(600, 450), "Identify your classmates!"),
      firstSelectedId(-1), secondSelectedId(-1), remainingPairs(4),
      timeLimit(20.0f), isGameActive(true), gameStarted(false),
      gameResult(false) {
          
    window.setFramerateLimit(60);
    window.requestFocus();

    if (!font.loadFromFile("/System/Library/Fonts/STHeiti Light.ttc")) {
        throw runtime_error("無法載入字型");
    }

    instructionText.setFont(font);
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::White);
    
    timerText.setFont(font);
    timerText.setCharacterSize(24);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(10, 10);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::Red);

    initializeCards();
    checkSpecialConditions();
}

// 建立和放置所有卡片
void CardMatch::initializeCards() {
    vector<pair<string, string>> cardPairs = {
        {"sources/card1 2.jpg", "sources/card2 2.jpg"},
        {"sources/card3 2.jpg", "sources/card4 2.jpg"},
        {"sources/card5 2.jpg", "sources/card6 2.jpg"},
        {"sources/card7 2.jpg", "sources/card8 2.jpg"}
    };

    int id = 0;
    for (int i = 0; i < cardPairs.size(); ++i) {
        cards.push_back(make_unique<Card>(id++, i, cardPairs[i].first));
        cards.push_back(make_unique<Card>(id++, i, cardPairs[i].second));
    }

    auto rng = default_random_engine(random_device{}());
    shuffle(cards.begin(), cards.end(), rng);

    float xOffset = 70, yOffset = 70;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 4; ++j) {
            int index = i * 4 + j;
            cards[index]->setPosition(
                xOffset + j * (cards[index]->getSize().x + 20),
                yOffset + i * (cards[index]->getSize().y + 20)
            );
        }
    }
}

// 處理滑鼠點擊
void CardMatch::handleMouseClick(const sf::Event::MouseButtonEvent& mouseButton) {
    if (mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (size_t i = 0; i < cards.size(); ++i) {
            if (cards[i]->getGlobalBounds().contains(
                static_cast<float>(mousePos.x),
                static_cast<float>(mousePos.y))
            ) {
                selectCard(i);
                break;
            }
        }
    }
}

// 選擇一張卡片
void CardMatch::selectCard(size_t index) {
    if (cards[index]->getIsMatched() || cards[index]->getIsRevealed()) {
        return;
    }

    cards[index]->reveal();

    if (firstSelectedId == -1) {
        firstSelectedId = index;
    }
    else {
        secondSelectedId = index;
        checkMatch();
    }
}

// 處理所有遊戲事件
void CardMatch::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed && isGameActive) {
            handleMouseClick(event.mouseButton);
        }
    }
}

// 檢查兩張卡片是否配對成功
void CardMatch::checkMatch() {
    if (*cards[firstSelectedId] == *cards[secondSelectedId]) {
        cards[firstSelectedId]->match();
        cards[secondSelectedId]->match();
        remainingPairs--;
    }
    else {
        sf::Clock delayClock;
        while (delayClock.getElapsedTime().asSeconds() < 1.0f) {
            render();
        }
        cards[firstSelectedId]->hide();
        cards[secondSelectedId]->hide();
    }

    firstSelectedId = -1;
    secondSelectedId = -1;
}

// 更新遊戲狀態，包括時間和遊戲結果
void CardMatch::update() {
    float remainingTime = timeLimit - gameClock.getElapsedTime().asSeconds();
    
    if (remainingTime <= 0) {
        isGameActive = false;
        gameResult = false;
        gameOverText.setString("Oops! \nYou can't recognize your classmates!");
        gameOverText.setFillColor(sf::Color::Red);
    } else if (remainingPairs == 0 && !waitingForWin) {
        waitingClock.restart();
        waitingForWin = true;
    } else if (waitingForWin && waitingClock.getElapsedTime().asSeconds() >= 1.0f) {
        isGameActive = false;
        gameResult = true;
        gameOverText.setString("Congratulations! \nYou know your classmates well!");
        gameOverText.setFillColor(sf::Color::Green);
    }

    if (isGameActive) {
        timerText.setString("time left: " + to_string(static_cast<int>(remainingTime)) +
                          "\npairs left: " + to_string(remainingPairs));
    }
}

// 繪製遊戲畫面
void CardMatch::render() {
    if (isGameActive) {
        window.clear(sf::Color::Black);
        for (const auto& card : cards) {
            window.draw(*card);
        }
        window.draw(timerText);
    } else {
        window.clear(sf::Color::Black);
        sf::FloatRect textBounds = gameOverText.getLocalBounds();
        gameOverText.setPosition(
            (window.getSize().x - textBounds.width) / 2,
            (window.getSize().y - textBounds.height) / 2
        );
        window.draw(gameOverText);
    }
    window.display();
}

// 檢查並設定特殊遊戲條件
bool CardMatch::checkSpecialConditions() {
    bool hasSpecialConditions = false;
    string instructions = "Rule:\nFind the pictures that are from same person and match them,\ndouble click to pick card.\n";
    
    if (player.getFriendship() >= 30) {
        remainingPairs = 2;
        instructions += "\nSince your friendship > 30, your friend is going to help you\n you only have to get two pairs to win.\n";
        hasSpecialConditions = true;
    }

    if (enemies[3]->getLevel() >= 1) {
        for (auto& card : cards) {
            card->applyEffect(&grayscaleShader);
        }
        instructions += "\nSince you're too emo,\nyou can't see things clearly...\n";
        hasSpecialConditions = true;
        enemies[3]->attack(&player);
    }

    instructionText.setString(instructions);
    sf::FloatRect textBounds = instructionText.getLocalBounds();
    instructionText.setPosition(
        (window.getSize().x - textBounds.width) / 2,
        (window.getSize().y - textBounds.height) / 2
    );

    return hasSpecialConditions;
}

// 顯示遊戲說明
void CardMatch::showInstructions() {
    window.clear(sf::Color::Black);
    window.draw(instructionText);
    window.draw(sf::Text("Press Enter to start", font, 20));
    window.display();
}

// 啟動並運行遊戲
bool CardMatch::start() {
    showInstructions();
    while (!gameStarted && window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return false;
            }
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Return) {
                gameStarted = true;
                gameClock.restart();
            }
        }
    }

    while (window.isOpen()) {
        handleEvents();
        update();
        render();
        
        if (!isGameActive) {
            this_thread::sleep_for(chrono::seconds(1));
            window.close();
            return gameResult;
        }
    }
    return gameResult;
}

// 更新遊戲結果狀態
void CardMatch::updateStatus(bool win) {
    if (win) {
        player.updateFriendship(30);
        player.addItem("goodPersonCard");
        cout << "你贏得了這場遊戲！\n";
        cout << "你有更多 homies 了!!\n";
        cout << "你的 friendship 上升了 30\n";
        cout << "你在地板上找到了一張「好人卡」！\n";
    } else {
        player.updateFriendship(-30);
        enemies[3]->levelUp();
        cout << "你輸了這場遊戲！\n";
        cout << "你跟別人心結了...\n";
        cout << "你的 friendship 下降了 30\n";
        cout << "EMO 等級提升了!!!\n";
    }
}
