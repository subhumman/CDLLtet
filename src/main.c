#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include "tetris.h"
#include <windows.h>

Tetblocks figure_template[] = {
    // I
    0,0,0,0,
    1,1,1,1,
    0,0,0,0,
    0,0,0,0,
    // O
    1,1,0,0,
    1,1,0,0,
    0,0,0,0,
    0,0,0,0,
    // T
    0,1,0,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,
    // S
    0,1,1,0,
    1,1,0,0,
    0,0,0,0,
    0,0,0,0,
    // Z
    1,1,0,0,
    0,1,1,0,
    0,0,0,0,
    0,0,0,0,
    // J
    1,0,0,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0,
    // L
    0,0,1,0,
    1,1,1,0,
    0,0,0,0,
    0,0,0,0
};

void printGame(TetGame* tetg){
    TetField* field = tetg->field;
    TetFigure* figure = tetg->figure;
    for (int k = 0; k < 30; k++) printf("\n"); // clear screen by printing empty lines
    for (int i = 0; i < field->height; i++) {
        printf("|");
        for (int j = 0; j < field->width; j++) {
            int sym = 0;
            if (field->blocks[i*field->width + j].block_arr != 0) {
                sym = 1;
            } else {
                int x = j - figure->x;
                int y = i - figure->y;
                if (x >= 0 && x < figure->size && y >= 0 && y < figure->size) {
                    if (figure->blocks[y*figure->size + x].block_arr != 0) {
                        sym = 1;
                    }
                }
            }
            printf(sym ? "#" : " ");
        }
        printf("|\n");
    }
    // bottom border
    printf("+");
    for (int i = 0; i < field->width; i++) printf("-");
    printf("+\n");
    fflush(stdout);
};

int main(void){
    
    TetGame* game = StartGame(10, 20, 4, 7, figure_template);
    if(!game){
        printf("SERIOS ERROR");
        getchar();
        return 1;
    };
    
    TetPlayer player = {.action = TET_PLAYER_NOPE}; 
    game->player = &player;

    FILE* log = fopen("log.txt", "w");
    if(!log){
        printf("BEDA");
    }

    // Main game loop
    while(game->playing != GAMEOVER){
        if (log) {
            fprintf(log, "Состояние игры: %d\n", game->playing);
            fflush(log);
        }
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 224) { // arrow key prefix
                int arrow = _getch();
                switch (arrow) {
                    case 72: // up
                        player.action = TET_PLAYER_UP;
                        break;
                    case 80: // down
                        player.action = TET_PLAYER_DOWN;
                        break;
                    case 75: // left
                        player.action = TET_PLAYER_LEFT;
                        break;
                    case 77: // right
                        player.action = TET_PLAYER_RIGHT;
                        break;
                    default:
                        player.action = TET_PLAYER_NOPE;
                        break;
                }
            } else {
                player.action = TET_PLAYER_NOPE;
            }
        } else {
            player.action = TET_PLAYER_NOPE;
        }
        calculate_game(game);
        printGame(game);
        Sleep(50); // delay for smooth falling
    }

    // Finish
    if (log) {
        fprintf(log, "Игра завершена\n");
        fclose(log);
    };

    freeStartGame(game);
    printf("enter");
    while(getchar() != '\n'); // wait for Enter

    return 0;
};