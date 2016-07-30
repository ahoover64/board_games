#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <memory>
#include <stdio.h>

class Player {
public:
    Player();
    Player(const Player&);
    ~Player();
    virtual FILE* get_out_stream() = 0;
    virtual char* get_move() = 0;
};

#endif // __PLAYER_H__
