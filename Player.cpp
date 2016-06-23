#include "Player.h"
#include <stdlib.h>

class Player::Impl {
public:
    FILE* input;
    FILE* output;
};

Player::Player() : mImpl(new Impl()) {
    mImpl->input = stdin;
    mImpl->output = stdout;
}



Player::Player(FILE* input, FILE* output) : mImpl(new Impl()) {
    mImpl->input = input;
    mImpl->output = output;
}

Player::~Player() { delete mImpl; }

FILE* Player::get_out_stream() {
    return mImpl->output;
}

char* Player::get_move() {
    char* move = (char*) malloc(40);
    if (fgets(move, 40, mImpl->input) != NULL) {
        return move;
    }
    return NULL;
}
