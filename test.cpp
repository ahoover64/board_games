#include <memory>
#include "TicTacToe.h"
#include "Amazons.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    Amazons g = Amazons();
    Player p1(stdin, stdout);
    Player p2(stdin, stdout);
    printf("%d\n", g.add_player(p1));
    printf("%d\n", g.add_player(p2));
    int w = g.play_game();
    printf("Player %i is the winner\n", w);
}
