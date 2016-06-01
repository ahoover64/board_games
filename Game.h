#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include "Player.h"

class Game {
public:
    virtual int add_player(Player& p) = 0;
    virtual int play_game() = 0;
    virtual void print_game(FILE* stream) = 0;
    Game();
    Game(const Game&);
    Game& operator=(const Game&);
    ~Game();
private:
    class Impl;
    std::shared_ptr<Impl> mImpl;
};

#endif // __GAME_H__

