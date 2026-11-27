// How does it work?
// Singleton ensures:
// 1. Only one instance of a class can be created.
// 2. Other classes access that same instance through a common method.



#include <bits/stdc++.h>
using namespace std;

// ================= SINGLETON =================

class GameConfig {
private:
    int volume;
    int difficulty;

    // Private constructor
    GameConfig() {
        volume = 50;
        difficulty = 1;
        cout << "GameConfig created\n";
    }

public:

    // Prevent copying
    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;

    // Return the single object
    static GameConfig& getInstance() {
        static GameConfig instance;
        return instance;
    }

    void setVolume(int v) {
        volume = v;
    }

    void setDifficulty(int d) {
        difficulty = d;
    }

    void display() {
        cout << "Volume: " << volume << "\n";
        cout << "Difficulty: " << difficulty << "\n";
    }
};

// ================= PLAYER =================

class Player {
public:
    void changeVolume(int volume) {
        GameConfig::getInstance().setVolume(volume);
    }
};

// ================= ENEMY =================

class Enemy {
public:
    void checkGameSettings() {
        cout << "Enemy checking settings:\n";
        GameConfig::getInstance().display();
    }
};

// ================= GAME MANAGER =================

class GameManager {
public:
    void changeDifficulty(int difficulty) {
        GameConfig::getInstance().setDifficulty(difficulty);
    }

    void showSettings() {
        cout << "Game Manager settings:\n";
        GameConfig::getInstance().display();
    }
};

// ================= MAIN =================

int main() {

    Player player;
    Enemy enemy;
    GameManager manager;

    // Player changes volume
    player.changeVolume(90);

    // GameManager changes difficulty
    manager.changeDifficulty(3);

    // Enemy accesses the SAME configuration
    enemy.checkGameSettings();

    // GameManager also sees the same values
    manager.showSettings();

    return 0;
}
