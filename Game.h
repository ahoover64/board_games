#ifndef __GAME_H__
#define __GAME_H__

#include <memory>
#include "Player.h"

class Game {
public:
    virtual void print_instructions(FILE* stream) = 0;
    virtual int add_player(Player& p) = 0;
    virtual int play_game() = 0;
    virtual void print_game(FILE* stream) = 0;
    // Returns a copy of the board, so that the info cna't be changed
    virtual const int* get_board() = 0; 
    bool is_locked();
    Game();
    Game(const Game&);
    Game& operator=(const Game&);
    ~Game();
protected:
    void lock_game();
    void unlock_game();
private:
    class Impl;
    Impl* gameImpl;
};

#endif // __GAME_H__
