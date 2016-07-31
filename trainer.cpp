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
    int w = 0;

    int count = 0;
    int num_games = 1000;
    int p1_games = 0;
    int p2_games = 0;
    double* p1_move = p1.move_weights();
    double* p1_shoot = p1.shoot_weights();
    double* p2_move = p2.move_weights();
    double* p2_shoot = p2.shoot_weights();
    while (count < num_games) {
        w = g.play_game();
        if (w == 1) {
            p1_games++;
            // change the loser weights to see if they can beat the winner next time
            for (int i = 0; i < 4; i++) {
                p2_move[i] = (p2_move[i] + p1_move[i]) / 2
                                - (0.5 + ((double) rand() / RAND_MAX)) * (num_games - count) / 50;
                p2_shoot[i] = (p2_shoot[i] + p1_shoot[i]) / 2
                                - (0.5 + ((double) rand() / RAND_MAX)) * (num_games - count) / 50;
            }
        }
        if (w == 2) {
            p2_games++;
            // change the loser weights to see if they can beat the winner next time
            for (int i = 0; i < 4; i++) {
                p1_move[i] = (p2_move[i] + p1_move[i]) / 2
                                - (0.5 + ((double) rand() / RAND_MAX)) * (num_games - count) / 50;
                p1_shoot[i] = (p2_shoot[i] + p1_shoot[i]) / 2
                                - (0.5 + ((double) rand() / RAND_MAX)) * (num_games - count) / 50;
            }
        }
        count++;
    }

    printf("P1 won: %d / %d games\n", p1_games, num_games);
    printf("P2 won: %d / %d games\n", p2_games, num_games);

    printf("P1 move weights: %.2f, %.2f, %.2f, %.2f\n", p1_move[0],
                                                        p1_move[1],
                                                        p1_move[2],
                                                        p1_move[3]);
    printf("P1 shoot weights: %.2f, %.2f, %.2f, %.2f\n", p1_shoot[0],
                                                         p1_shoot[1],
                                                         p1_shoot[2],
                                                         p1_shoot[3]);

    printf("P2 move weights: %.2f, %.2f, %.2f, %.2f\n", p2_move[0],
                                                        p2_move[1],
                                                        p2_move[2],
                                                        p2_move[3]);
    printf("P2 shoot weights: %.2f, %.2f, %.2f, %.2f\n", p2_shoot[0],
                                                         p2_shoot[1],
                                                         p2_shoot[2],
                                                         p2_shoot[3]);
}
