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
}

Amazons::Amazons(int n) : mImpl(new Impl) {
    mImpl->p = malloc(sizeof(Player) * 2);
    mImpl->p_count = 0;
    mImpl->malloc_size = 2;
    mImpl->board = malloc(sizeof(int) * (n * n));
    mImpl->board_size = n;
}

Amazons::~Amazons() {
    free(mImpl->p);
    free(mImpl->board);
    delete mImpl;
}

int Amazons::add_player(Player& p) {
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

}

int Amazons::play_game() {

}
