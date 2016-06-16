#ifndef __AMAZONS_H__
#define __AMAZONS_H__

#include <iostream>
#include <stdlib.h>

class Amazons : public Game {
public:
    Amazons(); // Board size is set to 10 x 10
    Amazons(int n); //Construct with specified board size
    ~Amazons();
    int add_player(Player&);
    int play_game();
    void print_instructions(FILE* stream);
    void print_game(FILE* stream);

public:
    class Impl;
    Impl* mImpl;
};

#endif // __AMAZONS_H__
