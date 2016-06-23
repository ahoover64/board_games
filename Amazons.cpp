#include "Amazons.h"
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (x > y) ? x : y
#define MIN(x, y) (x < y) ? x : y

class Amazons::Impl {
public:
    Player *p;
    int p_count;
    int malloc_size;
    int *board;
    int board_size;

    bool can_move(int player) {
        int loc_r = -1;
        int loc_c = -1;
        for (int i = 0; i < board_size * board_size; i++) {
            if (board[i] == player+1) {
                loc_r = i / board_size;
                loc_c = i % board_size;
                break;
            }
        }
        int startr = (loc_r - 1 < 0) ? loc_r : loc_r-1;
        int startc = (loc_c - 1 < 0) ? loc_c : loc_c-1;
        int endr   = (loc_r + 1 > board_size) ? loc_r : loc_r+1;
        int endc   = (loc_c + 1 > board_size) ? loc_c : loc_c+1;
        for (int r = startr; r < endr; r++) {
            for(int c = startc; c < endc; c++) {
                if (board[board_size * r + c] == 0)
                    return true;
            }
        }
        return false;
    }

    bool make_move(int player, int r_to, int c_to, int r_arrow, int c_arrow) {
        printf("(%i, %i), (%i, %i)\n", r_to, c_to, r_arrow, c_arrow);
        int loc_r = -1;
        int loc_c = -1;
        for (int i = 0; i < board_size * board_size; i++) {
            if (board[i] == player+1) {
                loc_r = i / board_size;
                loc_c = i % board_size;
                break;
            }
        }
        printf("LOC: (%i, %i)\n", loc_r, loc_c);

        // Check that the number are within the bounds
        // Also check that the player is actually moving / the arrow is shot
        if (r_to < 0 || r_to > board_size || c_to < 0 || c_to > board_size ||
                r_arrow < 0 || r_arrow > board_size ||
                c_arrow < 0 || c_arrow > board_size ||
                (r_to == loc_r && c_to == loc_c) ||
                (r_arrow == r_to && c_arrow == c_to)) {
            printf("ERROR 1\n");
            return false;
        }

        // Check that the move is along one of the proper paths
        // Check that the arrow shot is also a proper path
        if ((abs(r_to - loc_r) != abs(c_to - loc_c) && r_to != loc_r && c_to != loc_c)
                || (abs(r_arrow - r_to) != abs(c_arrow - c_to) && r_arrow != r_to &&
                c_arrow != c_to)) {
            printf("ERROR 2\n");
            return false;
        }

        // Check that the movement path is clear
        int r = MIN(loc_r, r_to);
        int c = MIN(loc_c, c_to);
        while(r <= MAX(loc_r, r_to) || c <= MAX(loc_c, c_to)) {
            if (board[board_size * r + c] != 0 && r != loc_r && c != loc_c) {
                printf("ERROR 3: (%i, %i)\n", r, c);
                return false;
            }
            r++; c++;
            if (loc_r == r_to)
                r--;
            if (loc_c == c_to)
                c--;
        }


        // Check that the the player has a clear shot
        r = MIN(r_arrow, r_to);
        c = MIN(c_arrow, c_to);
        while(r <= MAX(r_arrow, r_to) || c <= MAX(c_arrow, c_to)) {
            if (board[board_size * r + c] != 0 && r != r_to && c != c_to) {
                printf("ERROR 4: (%i, %i)\n", r, c);
                return false;
            }
            r++; c++;
            if (r_arrow == r_to)
                r--;
            if (c_arrow == c_to)
                c--;
        }

        board[board_size * loc_r + loc_c] = 0;        // A free space
        board[board_size * r_to + c_to] = player + 1; // New player position
        board[board_size * r_arrow + c_arrow] = -1;   // New blocked position

        return true;
    }
};

Amazons::Amazons() : mImpl(new Impl) {
    mImpl->p = (Player*) malloc(sizeof(Player) * 2);
    mImpl->p_count = 0;
    mImpl->malloc_size = 2;
    mImpl->board = (int*) malloc(sizeof(int) * (5 * 5));
    mImpl->board_size = 5;
    for (int r = 0; r < mImpl->board_size; r++) {
        for (int c = 0; c < mImpl->board_size; c++) {
            mImpl->board[mImpl->board_size * r + c] = 0;
        }
    }
}

Amazons::Amazons(int n) : mImpl(new Impl) {
    mImpl->p = (Player*) malloc(sizeof(Player) * 2);
    mImpl->p_count = 0;
    mImpl->malloc_size = 2;
    mImpl->board = (int*) malloc(sizeof(int) * (n * n));
    mImpl->board_size = n;
    for (int r = 0; r < mImpl->board_size; r++) {
        for (int c = 0; c < mImpl->board_size; c++) {
            mImpl->board[mImpl->board_size * r + c] = 0;
        }
    }
}

Amazons::~Amazons() {
    free(mImpl->p);
    free(mImpl->board);
    delete mImpl;
}

int Amazons::add_player(Player& p) {
    if (mImpl->p_count >= mImpl->board_size) {
        fprintf(p.get_out_stream(), "Cannot add more players than the side"
                                    "of the board.\n");
        lock_game();
        return 0;
    }
    if (is_locked()) {
        fprintf(stderr, "Game is locked.\n");
        return 0;
    }
    if (mImpl->malloc_size <= mImpl->p_count) {
        mImpl->p = (Player*) realloc(mImpl->p, sizeof(Player) * (mImpl->p_count + 1));
        mImpl->malloc_size = mImpl->p_count + 1;
    }
    mImpl->p[mImpl->p_count] = p;
    mImpl->p_count++;
    return 1;
}

void Amazons::print_instructions(FILE* stream) {
    if(!stream) {
        fprintf(stderr, "Stream can't be null\n");
    }
    fprintf(stream, "Input moves in the form: (r, c), (r, c) where the first \n"
                    "ordered pair is your move and the second your arrow.\n");
}

void Amazons::print_game(FILE* stream) {
    if(!stream) {
        fprintf(stderr, "Stream can't be null");
    }
    fprintf(stream, "  ");
    for(int i = 0; i < 4 * mImpl->board_size + 1; i++)
        fprintf(stream, "-");
    fprintf(stream, "\n");
    for (int r = 0; r < mImpl->board_size; r++) {
        fprintf(stream, "%i | ", r);
        for (int c = 0; c < mImpl->board_size; c++) {
            if (mImpl->board[mImpl->board_size * r + c] == -1)
                fprintf(stream, "X | ");
            if (mImpl->board[mImpl->board_size * r + c] == 0)
                fprintf(stream, "  | ");
            if (mImpl->board[mImpl->board_size * r + c] > 0)
                fprintf(stream, "%i | ", mImpl->board[mImpl->board_size * r + c]);
        }
        fprintf(stream, "\n  ");
        for(int i = 0; i < 4 * mImpl->board_size + 1; i++)
            fprintf(stream, "-");
        fprintf(stream, "\n");
    }
    fprintf(stream, "   ");
    for (int i = 0; i < mImpl->board_size; i++) {
        fprintf(stream, " %i  ", i);
    }
    fprintf(stream, "\n");
}

int Amazons::play_game() {
    int row, col;
    // Randomize player positions initially
    for (int p = 0; p < mImpl->p_count; p++) {
        row = rand() % mImpl->board_size;
        col = rand() % mImpl->board_size;
        if ( mImpl->board[mImpl->board_size * row + col] == 0){
            mImpl->board[mImpl->board_size * row + col] = (p+1);
        }
        else {
            p--;
        }
    }
    lock_game();
    int player = 0;
    int winner = 0;
    int p_with_move = mImpl->p_count;
    bool *has_move = (bool*) malloc(mImpl->p_count * sizeof(bool));
    for (int i = 0; i < mImpl->p_count; i++)
        has_move[i] = true;
    int move = 0;
    int r_to, c_to, r_arrow, c_arrow;
    while (p_with_move > 1) {
        player = move % mImpl->p_count;
        if (!mImpl->can_move(player)) {
            fprintf(mImpl->p[player].get_out_stream(), "Player %i has no valid "
                            "move. Their turn will be skipped.\n", player+1);
            p_with_move--;
            has_move[player] = false;
            move++;
            break;
        }
        else {
            if (!has_move[player]) {
                has_move[player] = true;
                p_with_move++;
            }
            print_game(mImpl->p[player].get_out_stream());
            char* s = mImpl->p[player].get_move();
            char* tok = strtok(s, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &r_to)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player].get_out_stream());
            }
            tok = strtok(NULL, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &c_to)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player].get_out_stream());
            }
            tok = strtok(NULL, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &r_arrow)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player].get_out_stream());
            }
            tok = strtok(NULL, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &c_arrow)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player].get_out_stream());
            }
            free(s);
            if (!mImpl->make_move(player, r_to, c_to, r_arrow, c_arrow)) {
                fprintf(stderr, "Try again, must be a valid move.\n");
                print_instructions(mImpl->p[player].get_out_stream());
            }
            else {
                move++;
            }
        }
    }
    winner = 0;
    free(has_move);
    while(!mImpl->can_move(winner))  {
        winner++;
    }
    winner++;
    unlock_game();
    return winner;
}
