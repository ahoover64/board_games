#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <memory>
#include <stdio.h>

class Player {
public:
    Player();
    Player(FILE* input, FILE* output);
    ~Player();
    virtual FILE* get_out_stream();
    virtual char* get_move();
private:
    class Impl;
    Impl* mImpl;
//    std::shared_ptr<Impl> mImpl;
};

#endif // __PLAYER_H__
