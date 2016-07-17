#include "TicTacToe.h"
#include <stdlib.h>

class TicTacToe::Impl {
public:
    Player* p;
    int board[9];
    int move;
    int players;
    int make_move(int player, int m) {
        if(m > 9 || board[m-1] != 0)
            return 0;
        else
            board[m-1] = (player == 0) ? 1 : -1;
        return 1;
    }

    int game_done() {
       for (int i = 0; i < 3; i++) {
            if (board[3*i] != 0 && board[3*i + 0] == board[3*i + 1] && board[3*i] == board[3*i + 2])
                return board[3*i];
            else if (board[i] != 0 && board[3*0 + i] == board[3*1 + i] && board[3*i] == board[3*2 + i])
                return board[i];
            else if (i != 2 && board[4] != 0 && (board[i*2] == board[4]) && (board[4] == board[8-i*2]))
                return board[4];
        }
        for (int i = 0; i < 9; i++) {
            if (board[i] == 0)
                return 0;
        }
        return 2; // Game is a draw
    }
};

TicTacToe::TicTacToe() : Game(), mImpl(new Impl()) {
    mImpl->p = (Player*) malloc(sizeof(Player) * 2);
    for (int i = 0; i < 9; i++)
        mImpl->board[i] = 0;
    mImpl->move = 0;
    mImpl->players = 0;
}

//TicTacToe::TicTacToe(const TicTacToe& g) {}

TicTacToe& TicTacToe::operator=(const TicTacToe& g) {}

TicTacToe::~TicTacToe() {
    free(mImpl->p);
    delete mImpl;
}

int TicTacToe::add_player(Player& p) {
    if (is_locked()) {
        fprintf(stderr, "Game is locked.");
    }
    if (mImpl->players == 0) {
        mImpl->p[0] = p;
        mImpl->players = 1;
    }
    else if (mImpl->players == 1) {
        mImpl->p[1] = p;
        mImpl->players = 2;
    }
    else {
        fprintf(stderr, "Can only add 2 players to TicTactoe.\n");
        return -1;
    }
    return 1;
}

void TicTacToe::print_instructions(FILE *stream) {
    if(!stream) {
        fprintf(stderr, "Stream can't be null.\n");
        return;
    }
    fprintf(stream, "Enter the number of the slot you wish to place your marker.\n");
}

void TicTacToe::print_game(FILE* stream) {
    if (!stream) {
        fprintf(stderr, "Stream can't be null.\n");
        return;
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (mImpl->board[3*i + j] == 0) {
                fprintf(stream, "%i", 3*i + j + 1);
            }
            else if (mImpl->board[3*i + j] == 1) {
                fprintf(stream, "x");
            }
            else if (mImpl->board[3*i + j] == -1) {
                fprintf(stream, "o");
            }
            if (j < 2)
                fprintf(stream, "|");
        }
        if (i < 2)
            fprintf(stream, "\n-----");
        fprintf(stream, "\n");
    }
}

int TicTacToe::play_game() {
    if(mImpl->players < 2) {
        fprintf(stderr, "Not enough players to start TicTacToe.");
        return -1;
    }
    int m = 0;
    int player = 0;
    int winner = 0;
    lock_game();
    while (!(winner = mImpl->game_done())) {
        player = mImpl->move % 2;
        print_game(mImpl->p[player].get_out_stream());
        char* s = mImpl->p[player].get_move();
        sscanf(s, "%i", &m);
        free(s);
        if(!mImpl->make_move(player, m)) {
            fprintf(mImpl->p[player].get_out_stream(), "Try again. Must be a valid move.\n");
        }
        else {
            mImpl->move++;
        }
    }
    if (winner == 2)
        return 0;
    unlock_game();
    return winner;
}

int* TicTacToe::get_board() {
    int* board_cp = (int*) malloc(sizeof(int) * 9);
    memcpy(board_cp, mImpl->board, sizeof(int) * 9);
    return board_cp;
}
