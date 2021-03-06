#ifndef __CHESS_H__
#define __CHESS_H__

#include <memory>

#include "Game.h"

class Chess : public Game {
public:
    Chess();
    Chess(const Chess&);
    Chess& operator=(const Chess&);
    ~Chess();
    int add_player(Player&);
    int play_game();
    void print_instructions(FILE* stream);
    void print_game(FILE* stream);
    int* get_board();
private:
    class Impl;
    std::shared_ptr<Impl> mImpl;
};

#endif // __CHESS_H__
