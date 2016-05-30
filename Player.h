#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <memory.h>
#include <stdio.h>

class Player {
public:
    Player();
    Player(FILE* input);
    Player& operator=(const Player&);
    ~Player();
    char* get_move();
private:
    class Impl;
    std::shared_ptr<Impl> mImpl;
};

#endif // __PLAYER_H__
