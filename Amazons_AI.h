/*
 * This AI uses random moves and weighs the outcomes of the moves based
 * on the results of the game. Or the result of the state.
 * ^ That's ideal, currently the AI is randomized evertime
 * Omce I have that working, I will trya dn get it to optimize itself
 */

#ifndef __AMAZONS_AI_H__
#define __AMAZONS_AI_H__

#include "Player.h"
#include "Game.h"

class AmazonsAI : private Player {
public:
    AmazonsAI(int p_num, int board_size);
    AmazonsAI(int p_num, int size, double [] move_weights, double [] shoot_weights);
    //AmazonsAI(const AmazonsAI&);
    //AmazonsAI& operator=(const AmazonsAI&);
    ~AmazonsAI();
    double* move_weights();
    double* shoot_weights();
    FILE* get_out_stream();
    char* get_move();
private:
    class Impl;
    Impl* aImpl;
};

#endif // __AMAZONS_AI_H__
