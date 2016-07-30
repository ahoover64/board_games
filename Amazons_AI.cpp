#include "Amazons_AI.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <string.h>

class AmazonsAI::Impl {
public:
    int num;
    const int* board;
    double* move_board;
    double* shoot_board;
    int board_size;
    // Weights to evaluate different moves
    double move_weights[4]; // In order: moves, blocks, closeness, farness
    double shoot_weights[4]; // In order: moves, blocks, closeness, farness

    // recycled from Impl class in Amazons
    int can_move(int loc) {
        int loc_r = loc / board_size;
        int loc_c = loc % board_size;
        int startr = (loc_r - 1 < 0) ? loc_r : loc_r-1;
        int startc = (loc_c - 1 < 0) ? loc_c : loc_c-1;
        int endr   = (loc_r + 1 > board_size) ? loc_r : loc_r+1;
        int endc   = (loc_c + 1 > board_size) ? loc_c : loc_c+1;
        for (int r = startr; r < endr; r++) {
            for(int c = startc; c < endc; c++) {
                if (board[board_size * r + c] == 0)
                    return 1;
            }
        }
        return 0;
    }

    // temporary method for testing. needs to be improved
    int num_blocked(int loc) {
        int loc_r = loc / board_size;
        int loc_c = loc % board_size;
        int startr = (loc_r - 1 < 0) ? loc_r : loc_r-1;
        int startc = (loc_c - 1 < 0) ? loc_c : loc_c-1;
        int endr   = (loc_r + 1 > board_size) ? loc_r : loc_r+1;
        int endc   = (loc_c + 1 > board_size) ? loc_c : loc_c+1;

        int blocked = 0;
        for (int r = startr; r < endr; r++) {
            for(int c = startc; c < endc; c++) {
                if (board[board_size * r + c] > 0)
                    blocked++;
            }
        }
        return blocked;
    }

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
        int pos = -1;
        move_board[loc] = -100; // arbitrary low value so that it doesn't try to stay in the same place
        for (col = 0; col < board_size; col++) {
            pos = row * board_size + col;
            if (pos == loc) continue;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
        }

        // Iterate along the column
        col = loc % board_size;
        pos = -1;
        for (row = 0; row < board_size; row++) {
            pos = row * board_size + col;
            if (pos == loc) continue;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) / board_size +
                              move_weights[3] * (board_size - ((pos - loc) / board_size));
        }

        // Iterate on main diagonal
        row = loc / board_size;
        col = loc % board_size;
        pos = -1;
        {
        int i;
        int c;
        for (i = std::abs(row - col), c = 0; i < board_size; i++, c++) {
            if (row > col) {
                pos = board_size * c + i;
            }
            else {
                pos = board_size * i + c;
            }
            if (pos == loc) continue;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) / board_size +
                              move_weights[3] * (board_size - (std::abs(pos - loc) / board_size));
        }
        }

        // Iterate on off diagonal
        pos = -1;
        if (row + col > board_size) {
            pos = board_size * (board_size-1) + (row + col - (board_size-1));
        }
        else {
            pos = row + col;
        }
        for (; pos > 0 && (pos / board_size != pos + (board_size - 1)); pos = pos - (board_size - 1)) {
            if (pos == loc) continue;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) / board_size +
                              move_weights[3] * (board_size - (std::abs(pos - loc) / board_size));
        }
    }

    // uses the information from find_move, so it should be executed after
    void find_shot() {
        int loc = 0;
        for (int i = 0; i < board_size  * board_size; i++) {
            loc = (move_board[i] > move_board[loc]) ? i : loc;
        }

        int row = loc / board_size;
        int col = 0;
        int pos = -1;
        shoot_board[loc] = -100; // arbitrary low value so that it doesn't try to stay in the same place
        for (col = 0; col < board_size; col++) {
            pos = row * board_size + col;
            if (pos == loc) continue;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
        }

        // Iterate along the column
        col = loc % board_size;
        pos = -1;
        for (row = 0; row < board_size; row++) {
            pos = row * board_size + col;
            if (pos == loc) continue;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) / board_size +
                               shoot_weights[3] * (board_size - ((pos - loc) / board_size));
        }

        // Iterate on main diagonal
        row = loc / board_size;
        col = loc % board_size;
        pos = -1;
        {
        int i;
        int c;
        for (i = std::abs(row - col), c = 0; i < board_size; i++, c++) {
            if (row > col) {
                pos = board_size * c + i;
            }
            else {
                pos = board_size * i + c;
            }
            if (pos == loc) continue;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) / board_size +
                               shoot_weights[3] * (board_size - (std::abs(pos - loc) / board_size));
        }
        }

        // Iterate on off diagonal
        pos = -1;
        if (row + col > board_size) {
            pos = board_size * (board_size-1) + (row + col - (board_size-1));
        }
        else {
            pos = row + col;
        }
        for (; pos > 0 && (pos / board_size != pos + (board_size - 1)); pos = pos - (board_size - 1)) {
            if (pos == loc) continue;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) / board_size +
                               shoot_weights[3] * (board_size - (std::abs(pos - loc) / board_size));
        }
    }

    char* get_move() {
        int best_move = 0;
        for (int i = 0; i < board_size  * board_size; i++) {
            best_move = (move_board[i] > move_board[best_move]) ? i : best_move;
        }

        int best_shot = 0;
        for (int i = 0; i < board_size  * board_size; i++) {
            best_shot = (shoot_board[i] > shoot_board[best_shot]) ? i : best_shot;
        }

        char* move = (char*) malloc(sizeof(char) * 20);
        sprintf(move, "(%d, %d), (%d, %d)", best_move / board_size,
                                            best_move % board_size,
                                            best_shot / board_size,
                                            best_shot % board_size);
        //printf("%s\n", move);
        return move;
    }
};

AmazonsAI::AmazonsAI(int p_num, int size, const int* game_board) : Player(), aImpl(new Impl) {
    aImpl->num = p_num;
    aImpl->board_size = size;
    aImpl->board = game_board;
    srand(time(NULL));
    aImpl->move_board = (double*) malloc(sizeof(double) * size * size);
    aImpl->shoot_board = (double*) malloc(sizeof(double) * size * size);
    for (int i = 0; i < 4; i++) {
        // Randomize between -1 and 2
        aImpl->move_weights[i] = -1.0 + ((double) rand() / RAND_MAX) * 3.0;
        aImpl->shoot_weights[i] = -1.0 + ((double) rand() / RAND_MAX) * 3.0;
    }
}

AmazonsAI::AmazonsAI(int p_num, int size, const int* game_board, double move_weights[],
                          double shoot_weights[]) : Player(), aImpl(new Impl) {
    aImpl->num = p_num;
    aImpl->board_size = size;
    aImpl->board = game_board;
    aImpl->move_board = (double*) malloc(sizeof(double) * size * size);
    aImpl->shoot_board = (double*) malloc(sizeof(double) * size * size);
    for (int i = 0; i < 4; i++) {
        aImpl->move_weights[i] = move_weights[i];
        aImpl->shoot_weights[i] = shoot_weights[i];
    }
}

//AmazonsAI::AmazonsAI(const AmazonsAI& other) {}

//AmazonsAI& AmazonsAI::operator=(const AmazonsAI& other) {}

AmazonsAI::~AmazonsAI() {
    free(aImpl->move_board);
    free(aImpl->shoot_board);
    delete aImpl;
}

FILE* AmazonsAI::get_out_stream() {
    return stdout;
}

double* AmazonsAI::move_weights() {
    return &(aImpl->move_weights[0]);
}

double* AmazonsAI::shoot_weights() {
    return &(aImpl->shoot_weights[0]);
}

char* AmazonsAI::get_move() {
    /*if (!aImpl->board) {
        free(aImpl->board);
    }
    aImpl->board = get_board();*/
    aImpl->find_move();
    aImpl->find_shot();
    return aImpl->get_move();
}
