#include "Amazons_AI.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cmath>
#include <climits>
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
            }
            move_board[i] = INT_MIN;
        }
        if (loc == -1) {
            fprintf(stderr, "player number was invalid\n");
        }

        // iter right
        int pos = loc + 1;
        while(pos % board_size != 0) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos++;
        }

        // iter down right
        pos = loc + board_size + 1;
        while(pos % board_size != 0 && pos < board_size * board_size) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos + board_size + 1;
        }

        // iter down
        pos = loc + board_size;
        while(pos < board_size * board_size) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos + board_size;
        }

        // iter down left
        pos = loc + board_size - 1;
        while(pos % board_size != (board_size-1) && pos < board_size * board_size) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos + board_size - 1;
        }

        // iter left
        pos = loc - 1;
        while(pos % board_size != (board_size-1) && pos > 0) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos--;
        }

        // iter up left
        pos = loc - board_size - 1;
        while(pos % board_size != (board_size-1) && pos > 0) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos - board_size - 1;
        }

        // iter up
        pos = loc - board_size;
        while(pos > 0) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos - board_size;
        }

        // iter up right
        pos = loc - board_size + 1;
        while(pos % board_size != 0 && pos > 0) {
            if (board[pos] != 0) break;
            move_board[pos] = move_weights[0] * can_move(pos) +
                              move_weights[1] * num_blocked(pos) +
                              move_weights[2] * std::abs(pos - loc) +
                              move_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos - board_size + 1;
        }
    }

    // uses the information from find_move, so it should be executed after
    void find_shot() {
        int loc = 0;
        for (int i = 0; i < board_size  * board_size; i++) {
            loc = (move_board[i] > move_board[loc]) ? i : loc;
            shoot_board[i] = INT_MIN;
        }
        shoot_board[loc] = INT_MIN; // arbitrary low value so that it doesn't try to stay in the same place

        int pos = loc + 1;
        while(pos % board_size != 0) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos++;
        }

        // iter down right
        pos = loc + board_size + 1;
        while(pos % board_size != 0 && pos < board_size * board_size) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos + board_size + 1;
        }

        // iter down
        pos = loc + board_size;
        while(pos < board_size * board_size) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos + board_size;
        }

        // iter down left
        pos = loc + board_size - 1;
        while(pos % board_size != (board_size-1) && pos < board_size * board_size) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos + board_size - 1;
        }

        // iter left
        pos = loc - 1;
        while(pos % board_size != (board_size-1) && pos > 0) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos--;
        }

        // iter up left
        pos = loc - board_size - 1;
        while(pos % board_size != (board_size-1) && pos > 0) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos - board_size - 1;
        }

        // iter up
        pos = loc - board_size;
        while(pos > 0) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos - board_size;
        }

        // iter up right
        pos = loc - board_size + 1;
        while(pos % board_size != 0 && pos > 0) {
            if (board[pos] != 0 && board[pos] != num) break;
            shoot_board[pos] = shoot_weights[0] * can_move(pos) +
                               shoot_weights[1] * num_blocked(pos) +
                               shoot_weights[2] * std::abs(pos - loc) +
                               shoot_weights[3] * (board_size - std::abs(pos - loc));
            pos = pos - board_size + 1;
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
    return NULL;
}

double* AmazonsAI::move_weights() {
    return &(aImpl->move_weights[0]);
}

double* AmazonsAI::shoot_weights() {
    return &(aImpl->shoot_weights[0]);
}

char* AmazonsAI::get_move() {
    aImpl->find_move();
    aImpl->find_shot();
    return aImpl->get_move();
}
