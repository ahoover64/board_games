#include <memory>
#include "TicTacToe.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    TicTacToe g = TicTacToe();
    Player p1(stdin, stdout);
    Player p2(stdin, stdout);
    g.add_player(p1);
    g.add_player(p2);
    int w = g.play_game();
    printf("%i is the winner\n", w);
}
