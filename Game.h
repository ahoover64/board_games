#ifndef __GAME_H__
#define __GAME_H__

#include <memory.h>
#include "Player.h"

class Game {
public:
    virtual int add_player(const Player& p) = 0;
    virtual void play_game() = 0;
private:
    Game();
    Game(const Game&);
    Game& operator=(const Game&);
    ~Game();
    class Impl;
    std::shared_ptr<Impl> mImpl;
};

#endif // __GAME_H__
