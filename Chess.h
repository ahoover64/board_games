#ifndef __CHESS_H__
#define __CHESS_H__

#include <memory.h>

#include "Game.h"

class Chess : Game {
public:
    Chess();
    Chess(const Chess&);
    Chess& operator=(const Chess&);
    ~Chess();
    int add_player(const Player&);
    void play_game();

private:
    class Impl;
    std::shared_ptr<Impl> mImpl;
};

#endif // __CHESS_H__
