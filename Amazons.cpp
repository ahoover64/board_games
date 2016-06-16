#include "Amazons.h"
#include <stdlib.h>

class Amazons::Impl {
    Player *p;
    int p_count;
    int malloc_size;
    int *board;
    int board_size;
};

Amazons::Amazons() : mImpl(new Impl) {
    mImpl->p = malloc(sizeof(Player) * 2);
    mImpl->p_count = 0;
    mImpl->malloc_size = 2;
    mImpl->board = malloc(sizeof(int) * (10 * 10));
    mImpl->board_size = 10;
    for (int r = 0; r < mImpl->board_size; r++) {
        for (int c = 0; c < mImpl->board_size; c++) {
            board[mImpl->board_size * r + c] = 0;
        }
    }
}

Amazons::Amazons(int n) : mImpl(new Impl) {
    mImpl->p = malloc(sizeof(Player) * 2);
    mImpl->p_count = 0;
    mImpl->malloc_size = 2;
    mImpl->board = malloc(sizeof(int) * (n * n));
    mImpl->board_size = n;
    for (int r = 0; r < mImpl->board_size; r++) {
        for (int c = 0; c < mImpl->board_size; c++) {
            board[mImpl->board_size * r + c] = 0;
        }
    }
}

Amazons::~Amazons() {
    free(mImpl->p);
    free(mImpl->board);
    delete mImpl;
}

int Amazons::add_player(Player& p) {
    if (is_locked()) {
        fprintf(stderr, "Game is locked");
    }
    if (malloc_size <= p_count) {
        realloc(mImpl->p, sizeof(Player) * (p_count + 1));
        malloc_size = p_count + 1;
    }
    p[p_count] = p;
    p_count++;
}

void Amazons::print_instructions(FILE* stream) {
    if(!stream) {
        fprintf(stderr, "Stream can't be null");
    }
    fprintf(stream, "Input moves in the form: (r, c), (r, c) where the
                    first ordered pair is your move and the second your arrow.");
}

void Amazons::print_game(FILE* stream) {
    if(!stream) {
        fprintf(stderr, "Stream can't be null");
    }
    for (int r = 0; r < mImpl->board_size; r++) {
        for (int c = 0; c < mImpl->board_size; c++) {

        }
    }
}

int Amazons::play_game() {
    int row, col;
    // Randomize player positions initially
    for (int p = 0; p < mImpl->p_count; p++) {
        row = rand() % mImpl->board_size;
        col = rand() % mImpl->board_size;
        if ( board[mImpl->board_size * row + col] == 0){
            board[mImpl->board_size * row + col] = (p+1);
        }
        else {
            p--;
        }
    }
    lock_game();
    int player = 0;
    int winner = 0;
    int no_move_cnt = 0;
    int move = 0;
    while (no_move_cnt < mImpl->p_count-1) {
        player = move % mImpl->p_count;
        if (!mImpl->can_move(player)) {
            fprintf(mImpl->p[player].get_out_stream(), "Player %i has no valid move", player+1);
            move++;
            break;
        }
        move++;
    }
    unlock_game();
    return winner;
}
