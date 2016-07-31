#include "Amazons.h"
#include <stdlib.h>
#include <string.h>

#define MAX(x, y) (x > y) ? x : y
#define MIN(x, y) (x < y) ? x : y

class Amazons::Impl {
public:
    Player **p;
    int p_count;
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
        int endr   = (loc_r + 1 >= board_size) ? loc_r : loc_r+1;
        int endc   = (loc_c + 1 >= board_size) ? loc_c : loc_c+1;
        for (int r = startr; r <= endr; r++) {
            for(int c = startc; c <= endc; c++) {
                if (board[board_size * r + c] == 0)
                    return true;
            }
        }
        return false;
    }

    bool make_move(int player, int r_to, int c_to, int r_arrow, int c_arrow) {
        int loc_r = -1;
        int loc_c = -1;
        for (int i = 0; i < board_size * board_size; i++) {
            if (board[i] == player+1) {
                loc_r = i / board_size;
                loc_c = i % board_size;
                break;
            }
        }

        // Check that the number are within the bounds
        // Also check that the player is actually moving / the arrow is shot
        if (r_to < 0 || r_to > board_size || c_to < 0 || c_to > board_size ||
                r_arrow < 0 || r_arrow > board_size ||
                c_arrow < 0 || c_arrow > board_size ||
                (r_to == loc_r && c_to == loc_c) ||
                (r_arrow == r_to && c_arrow == c_to)) {
            printf("ERROR 1: (%d, %d), (%d, %d)\n", r_to, c_to, r_arrow, c_arrow);
            return false;
        }

        // Check that the move is along one of the proper paths
        // Check that the arrow shot is also a proper path
        if ((abs(r_to - loc_r) != abs(c_to - loc_c) && r_to != loc_r && c_to != loc_c)
                || (abs(r_arrow - r_to) != abs(c_arrow - c_to) && r_arrow != r_to &&
                c_arrow != c_to)) {
            printf("ERROR 2: (%d, %d), (%d, %d)\n", r_to, c_to, r_arrow, c_arrow);
            return false;
        }


        // Check that the movement path is clear
        int r = loc_r;
        int c = loc_c;
        int end_r = r_to;
        int end_c = c_to;
        while (r != end_r || c != end_c) {
            if (loc_r > r_to) r--;
            else if (loc_r < r_to) r++;

            if (loc_c > c_to) c--;
            else if (loc_c < c_to) c++;

            if (board[board_size * r + c] != 0) {
                printf("ERROR 3: (%d, %d), (%d, %d)\n", r_to, c_to, r_arrow, c_arrow);
                printf("Blocking: (%d, %d)\n", r, c);
                return false;
            }

        }

        // Check that the the player has a clear shot
        r = r_to;
        c = c_to;
        end_r = r_arrow;
        end_c = c_arrow;
        while (r != end_r || c != end_c) {
            if (r_to > r_arrow) r--;
            else if (r_to < r_arrow) r++;

            if (c_to > c_arrow) c--;
            else if (c_to < c_arrow) c++;

            if (board[board_size * r + c] != 0 &&
                board[board_size * r + c] != (player+1)) {
                printf("ERROR 4: (%d, %d), (%d, %d)\n", r_to, c_to, r_arrow, c_arrow);
                printf("Blocking: (%d, %d)\n", r, c);
                return false;
            }

        }

        board[board_size * loc_r + loc_c] = 0;        // A free space
        board[board_size * r_to + c_to] = player + 1; // New player position
        board[board_size * r_arrow + c_arrow] = -1;   // New blocked position

        return true;
    }
};

Amazons::Amazons() : mImpl(new Impl) {
    mImpl->p = new Player*[5];
    mImpl->p_count = 0;
    mImpl->board = (int*) malloc(sizeof(int) * (5 * 5));
    mImpl->board_size = 5;
    for (int r = 0; r < mImpl->board_size; r++) {
        for (int c = 0; c < mImpl->board_size; c++) {
            mImpl->board[mImpl->board_size * r + c] = 0;
        }
    }
}

Amazons::Amazons(int n) : mImpl(new Impl) {
    mImpl->p = new Player*[n];
    mImpl->p_count = 0;
    mImpl->board = (int*) malloc(sizeof(int) * (n * n));
    mImpl->board_size = n;
    for (int r = 0; r < mImpl->board_size; r++) {
        for (int c = 0; c < mImpl->board_size; c++) {
            mImpl->board[mImpl->board_size * r + c] = 0;
        }
    }
}

Amazons::~Amazons() {
    delete [] mImpl->p;
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
    mImpl->p[mImpl->p_count] = &p;
    mImpl->p_count++;
    return mImpl->p_count;
}

void Amazons::print_instructions(FILE* stream) {
    if(!stream) {
        return;
        //fprintf(stderr, "Stream can't be null\n");
    }
    fprintf(stream, "Input moves in the form: (r, c), (r, c) where the first \n"
                    "ordered pair is your move and the second your arrow.\n");
}

void Amazons::print_game(FILE* stream) {
    if(!stream) {
        return;
        //fprintf(stderr, "Stream can't be null\n");
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

    // Reset the board in case mutliple games are played
    for (int i = 0; i < mImpl->board_size * mImpl->board_size; i++) {
        mImpl->board[i] = 0;
    }
    // Randomize player positions initially
    srand(time(NULL));
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
        print_game(mImpl->p[player]->get_out_stream());
        if (!mImpl->can_move(player)) {
            if ((mImpl->p[player]->get_out_stream()) != NULL) {
                fprintf(mImpl->p[player]->get_out_stream(), "Player %i has no valid "
                            "move. Their turn will be skipped.\n", player+1);
            }
            p_with_move--;
            has_move[player] = false;
            move++;
        }
        else {
            if (!has_move[player]) {
                has_move[player] = true;
                p_with_move++;
            }
            char* s = mImpl->p[player]->get_move();
            char* tok = strtok(s, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &r_to)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player]->get_out_stream());
            }
            tok = strtok(NULL, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &c_to)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player]->get_out_stream());
            }
            tok = strtok(NULL, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &r_arrow)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player]->get_out_stream());
            }
            tok = strtok(NULL, " (),");
            if(tok == NULL || !sscanf(tok, "%i", &c_arrow)) {
                fprintf(stderr, "Invalid input format.\n");
                print_instructions(mImpl->p[player]->get_out_stream());
            }
            free(s);
            if (!mImpl->make_move(player, r_to, c_to, r_arrow, c_arrow)) {
                fprintf(stderr, "Try again, must be a valid move.\n");
                print_instructions(mImpl->p[player]->get_out_stream());
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
    if (mImpl->p_count == 2) {
        winner = move % 2;
    }
    winner++; // Adjust from index to count
    unlock_game();
    return winner;
}

// Unmutable copy of the Amazons board used for the Amazons AI
const int* Amazons::get_board() {
    //int* board_cp = (int*) malloc(sizeof(int) * mImpl->board_size * mImpl->board_size);
    //memcpy(board_cp, mImpl->board, sizeof(int) * mImpl->board_size * mImpl->board_size);
    return mImpl->board;
}
