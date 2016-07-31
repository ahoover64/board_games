/*
 * I will write a test program to try and optimize the current weights in the AmazonsAI
 * class. This will be to test my very rudamentary AI.
 */


#include <memory>
#include "Amazons.h"
#include "Amazons_AI.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    Amazons g = Amazons();
    AmazonsAI p1(1, 5, g.get_board());
    AmazonsAI p2(2, 5, g.get_board());
    g.add_player(p1);
    g.add_player(p2);
    int w = g.play_game();
    //printf("Player %i is the winner\n", w);
    int count = 0;
    double* move_dummy;
    double* shoot_dummy;
    while (count < 100) {
        w = g.play_game();
        if (w == 1) {
            move_dummy = p2.move_weights();
            shoot_dummy = p2.shoot_weights();
        }
        if (w == 2) {
            move_dummy = p1.move_weights();
            shoot_dummy = p1.shoot_weights();
        }
        // change the loser weights to see if they can beat the winner next time
        for (int i = 0; i < 4; i++) {
            move_dummy[i] = move_dummy[i] - 0.5 +((double) rand() / RAND_MAX);
            shoot_dummy[i] = move_dummy[i] - 0.5 +((double) rand() / RAND_MAX);
        }
        count++;
    }

    move_dummy = p1.move_weights();
    shoot_dummy = p1.shoot_weights();
    printf("Player 1 move weights: %.2f, %.2f, %.2f, %.2f\n", move_dummy[0],
                                                              move_dummy[1],
                                                              move_dummy[2],
                                                              move_dummy[3]);
    printf("Player 1 shoot weights: %.2f, %.2f, %.2f, %.2f\n", shoot_dummy[0],
                                                               shoot_dummy[1],
                                                               shoot_dummy[2],
                                                               shoot_dummy[3]);
    move_dummy = p2.move_weights();
    shoot_dummy = p2.shoot_weights();
    printf("Player 2 move weights: %.2f, %.2f, %.2f, %.2f\n", move_dummy[0],
                                                              move_dummy[1],
                                                              move_dummy[2],
                                                              move_dummy[3]);
    printf("Player 2 shoot weights: %.2f, %.2f, %.2f, %.2f\n", shoot_dummy[0],
                                                               shoot_dummy[1],
                                                               shoot_dummy[2],
                                                               shoot_dummy[3]);
}
