#include "Amazons_AI.h"
#include <stdlib.h>
#include <time.h>

class AmazonsAI::Impl {
public:
    int num;
    int* board;
    double* eval_board;
    int board_size;
    // Weights to evaluate different moves
    double weights[4]; // In order: moves, blocks, closeness, farness
    void find_move() {
        int loc = -1;
        for (int i = 0; i < board_size * board_size; i++) {
            if (board[i] == num) {
                loc = i;
                break;
            }
        }
        if (loc == -1) {
            fprintf(stderr, "player number was invalid\n");
        }
    }
    char* get_move() {

    }
};

AmazonsAI::AmazonsAI(int p_num, int size) : Player(), aImpl(new Impl) {
    aImpl->num = p_num;
    aImpl->board_size = size;
    aImpl->eval_board = malloc(sizeof(double) * size * size);
    srand(time(NULL));
    for (int i = 0; i < 4; i++) {
        // Randomize between -1 and 2
        aImpl->weights[i] = -1.0 + ((double) rand() / RAND_MAX) * 3.0;
    }
}

AmazonsAI::AmazonsAI(int p_num, int size, int[] weights) : Player(), aImpl(new Impl) {}

//AmazonsAI::AmazonsAI(const AmazonsAI& other) {}

//AmazonsAI& AmazonsAI::operator=(const AmazonsAI& other) {}

AmazonsAI::~AmazonsAI() {
    free(aImpl->board);
    delete aImpl;
}

FILE* AmazonsAI::get_out_stream() {
    return stdout;
}

char* AmazonsAI::get_move() {
    if (!aImpl->board) {
        free(aImpl->board);
    }
    aImpl->board = get_board();
    aImpl->find_move();
    return aImpl->get_move();
}
