/*
 * This AI uses random moves and weighs the outcomes of the moves based
 * on the results of the game. Or the result of the state.
 */

#ifndef __AMAZONS_AI_H__
#define __AMAZONS_AI_H__

#include "Player.h"
#include "Game.h"

class AmazonsAI : private Player {
public:
    AmazonsAI();
    AmazonsAI(FILE* previous);
    AmazonsAI(const AmazonsAI&);
    AmazonsAI& operator=(const AmazonsAI&);
    ~AmazonsAI();
private:
    class Impl;
    Impl* mImpl;
};

#endif // __AMAZONS_AI_H__
