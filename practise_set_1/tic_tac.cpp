
#include <bits/stdc++.h>
using namespace std;

class Game;

// ================= STATE INTERFACE =================

class GameState {
public:
    virtual void handle_move(Game& game, int row, int col) = 0;
    virtual bool is_finished() = 0;
    virtual void announce_result() = 0;

    virtual ~GameState() = default;
};

// ================= BOARD =================

class Board {
private:
    int m = 3, n = 3;
    vector<vector<char>> board;

public:
    Board() : board(m, vector<char>(n, ' ')) {}

    bool valid_position(int row, int col) {
        return row >= 0 && row < m &&
               col >= 0 && col < n;
    }

    bool is_empty(int row, int col) {
        return valid_position(row, col) &&
               board[row][col] == ' ';
    }

    void place_symbol(int row, int col, char symbol) {
        board[row][col] = symbol;
    }

    void display() {
        cout << "\n";

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                cout << board[i][j];
                if (j < n - 1) cout << " | ";
            }

            cout << "\n";
            if (i < m - 1) cout << "---------\n";
        }

        cout << "\n";
    }

    bool is_full() {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == ' ')
                    return false;
            }
        }

        return true;
    }

    bool check_winner(char symbol) {
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == symbol &&
                board[i][1] == symbol &&
                board[i][2] == symbol)
                return true;
        }

        for (int j = 0; j < 3; j++) {
            if (board[0][j] == symbol &&
                board[1][j] == symbol &&
                board[2][j] == symbol)
                return true;
        }

        if (board[0][0] == symbol &&
            board[1][1] == symbol &&
            board[2][2] == symbol)
            return true;

        if (board[0][2] == symbol &&
            board[1][1] == symbol &&
            board[2][0] == symbol)
            return true;

        return false;
    }
};


// ================= PLAYER =================
class Player {
protected:
    string name;
    char symbol;

public:
    Player(string name, char symbol)
        : name(name), symbol(symbol) {}

    string get_name() {
        return name;
    }

    char get_symbol() {
        return symbol;
    }
};

class PlayerX : public Player {
public:
    PlayerX(string name) : Player(name, 'X') {}
};

class PlayerO : public Player {
public:
    PlayerO(string name) : Player(name, 'O') {}
};

// ================= GAME =================

class Game {
private:
    Board board;
    PlayerX playerX;
    PlayerO playerO;
    Player* currentPlayer;
    shared_ptr<GameState> currentState;
public:
    Game(string nameX, string nameO)
        : playerX(nameX),
          playerO(nameO),
          currentPlayer(&playerX) {}

    void set_state(shared_ptr<GameState> state) {
        currentState = state;
    }

    void makeMove(int row, int col) {
        if (currentState)
            currentState->handle_move(*this, row, col);
    }

    bool valid_move(int row, int col) {
        return board.is_empty(row, col);
    }

    void place_move(int row, int col) {
        board.place_symbol(row, col, currentPlayer->get_symbol());
    }

    bool validate_win() {
        return board.check_winner(currentPlayer->get_symbol());
    }

    bool validate_draw() {
        return board.is_full();
    }

    void switch_player() {
        currentPlayer = (currentPlayer == &playerX)
                        ? static_cast<Player*>(&playerO)
                        : static_cast<Player*>(&playerX);
    }

    string get_current_player_name() {
        return currentPlayer->get_name();
    }

    char get_current_player_symbol() {
        return currentPlayer->get_symbol();
    }

    void display_board() {
        board.display();
    }

    void start() {
        while (!currentState->is_finished()) {
            display_board();

            int row, col;

            cout << get_current_player_name()
                 << " (" << get_current_player_symbol()
                 << ") - Enter row and column: ";

            cin >> row >> col;

            makeMove(row, col);
        }

        currentState->announce_result();
        display_board();
    }
};

// ================= PLAYING STATE =================

class PlayingState : public GameState {
public:
    void handle_move(Game& game, int row, int col) override {

        if (!game.valid_move(row, col)) {
            cout << "Invalid move! Try again.\n";
            return;
        }

        game.place_move(row, col);

        if (game.validate_win()) {
            game.set_state(
                make_shared<WinState>(game.get_current_player_name())
            );
            return;
        }

        if (game.validate_draw()) {
            game.set_state(make_shared<DrawState>());
            return;
        }

        game.switch_player();
    }

    bool is_finished() override {
        return false;
    }

    void announce_result() override {}
};

// ================= WIN STATE =================

class WinState : public GameState {
private:
    string winner;

public:
    WinState(string winner) : winner(winner) {}

    void handle_move(Game& game, int row, int col) override {
        cout << "Game already finished! No more moves allowed.\n";
    }

    bool is_finished() override {
        return true;
    }

    void announce_result() override {
        cout << winner << " has won the game!\n";
    }
};

// ================= DRAW STATE =================

class DrawState : public GameState {
public:
    void handle_move(Game& game, int row, int col) override {
        cout << "Game already finished! It is a draw.\n";
    }

    bool is_finished() override {
        return true;
    }

    void announce_result() override {
        cout << "Game ended in a draw!\n";
    }
};

// ================= MAIN =================

int main() {

    Game game("Player X", "Player O");

    game.set_state(make_shared<PlayingState>());

    game.start();

    return 0;
}
