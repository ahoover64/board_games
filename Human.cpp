#include "Human.h"
#include <stdlib.h>

class Human::Impl {
public:
    FILE* input;
    FILE* output;
};

Human::Human() : mImpl(new Impl()) {
    mImpl->input = stdin;
    mImpl->output = stdout;
}

Human::Human(FILE* input, FILE* output) : mImpl(new Impl()) {
    mImpl->input = input;
    mImpl->output = output;
}

Human::Human(const Human& p) : mImpl(new Impl()) {
    mImpl->input = p.mImpl->input;
    mImpl->output = p.mImpl->output;
}

Human::~Human() {
    delete mImpl;
}

FILE* Human::get_out_stream() {
    return mImpl->output;
}

char* Human::get_move() {
    char* move = (char*) malloc(40);
    if (fgets(move, 40, mImpl->input) != NULL) {
        return move;
    }
    return NULL;
}
