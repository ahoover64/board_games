#include "Game.h"

class Game::Impl {
public:
    bool locked;
};

Game::Game() : gameImpl(new Impl) { 
    gameImpl->locked = false;
}

Game::Game(const Game& g) : gameImpl(g.gameImpl) {}

Game::~Game() { delete gameImpl; }

bool Game::is_locked() {
    return gameImpl->locked;
}

void Game::lock_game() {
    gameImpl->locked = true;
}

void Game::unlock_game() {
    gameImpl->locked = false;
}
