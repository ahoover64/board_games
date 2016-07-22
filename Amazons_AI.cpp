#include "Amazons_AI.h"
#include <stdlib.h>
#include <time.h>

class AmazonsAI::Impl {
public:
    int num;
    int* board;
    double* move_board;
    double* shoot_board;
    int board_size;
    // Weights to evaluate different moves
    double move_weights[4]; // In order: moves, blocks, closeness, farness
    double shoot_weights[4]; // In order: moves, blocks, closeness, farness

    void find_move() {
        int loc = -1;
        for (int i = 0; i < board_size * board_size; i++) {
            if (board[i] == num) {
                loc = i;
                break;
            }
            move_board[i] = 0;
            shoot_board[i] = 0;
        }
        if (loc == -1) {
            fprintf(stderr, "player number was invalid\n");
        }

        // Iterate along the row
        int row = loc / board_size;
        int col = 0;
        for (col = 0; col < board_size; col++) {
            board[row * board_size + col];
        }

        // Iterate along the column
        col = loc % board_size;
        for (row = 0; row < board_size; row++) {
            board[row * board_size + col];
        }

        // Iterate on main diagonal

        // Iterate on off diagonal
    }

    char* get_move() {
        int best_move = 0;
        for (int i = 0; i < board_size  * board_size; i++) {
            best_move = (move_board[i] > move_board[best_move]) ? i : best_move;
        }

        int best_shot = -1;
    }
};

AmazonsAI::AmazonsAI(int p_num, int size) : Player(), aImpl(new Impl) {
    aImpl->num = p_num;
    aImpl->board_size = size;
    srand(time(NULL));
    aImpl->move_board = malloc(sizeof(double) * size * size);
    aImpl->shoot_board = malloc(sizeof(double) * size * size);
    for (int i = 0; i < 4; i++) {
        // Randomize between -1 and 2
        aImpl->move_weights[i] = -1.0 + ((double) rand() / RAND_MAX) * 3.0;
        aImpl->shoot_weights[i] = -1.0 + ((double) rand() / RAND_MAX) * 3.0;
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
