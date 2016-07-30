#ifndef __HUMAN_H__
#define __HUMAN_H__

#include <memory>
#include <stdio.h>
#include "Player.h"

class Human : public Player {
public:
    Human();
    Human(FILE* input, FILE* output);
    Human(const Human&);
    ~Human();
    FILE* get_out_stream();
    char* get_move();
private:
    class Impl;
    Impl* mImpl;
};

#endif // __HUMAN_H__
