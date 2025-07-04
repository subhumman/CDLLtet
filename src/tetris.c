#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef TETRIS_C
#define TETRIS_C

// Memory is freed in cleanup functions for all mallocs

TetField* createField(int width, int height){
    TetField* field = (TetField*)malloc(sizeof(TetField)); // allocate field
    field->width = width; // set width
    field->height = height; // set height
    field->blocks = (Tetblocks*)malloc(sizeof(Tetblocks)*width*height); // allocate blocks
    // we will be think that blocks is None of field -> blocks[i].block_arr = 0. (do it at loop)
    for(unsigned i = 0; i < width*height; i++)
        field->blocks[i].block_arr = 0; // empty block
    return field; // return field
};

// Memory is freed twice and in reverse order because memory was allocated twice
void freeFieldMemory(TetField* field){
    if(field){
        if(field->blocks)
            free(field->blocks);
        free(field);
    };
};

// this func about create figures template.
TetFiguresT* createFiguresT(int count, int figure_size, Tetblocks* figureT){
    TetFiguresT* figT = (TetFiguresT*)malloc(sizeof(TetFiguresT));
    figT->blocks = figureT;
    figT->count = count;
    figT->size = figure_size;
    return figT;
};

// func for memory free of figure template. Memory was allocated once, so it should be freed once. 
void freeFigTemplateMalloc(TetFiguresT* figure){
    if(figure)
        free(figure);
};

TetGame* StartGame(int field_w, int field_h, int figure_size, int count, Tetblocks* figure_templates){
    TetGame* tetg = (TetGame*)malloc(sizeof(TetGame)); // allocate game struct
    // For now, let's assume we already have methods that define the following structures.
    tetg->field = createField(field_w, field_h);
    tetg->figurest = createFiguresT(count, figure_size, figure_templates);
    tetg->ticks = TET_TICKS_START;
    tetg->left_ticks = TET_TICKS_START;
    tetg->playing = PLAYING;
    tetg->score = 0;
    return tetg;
};
 
// func for freed memory of all game. Once we freed struct components, twice - structure
void freeStartGame(TetGame* game){
    if(game){
        freeFieldMemory(game->field);
        freeFigTemplateMalloc(game->figurest);
        free(game);
    };
};


// now will most interest part - procedure programming for actions

void figureDrop(TetGame* tetg){
// for move figure down we must do y++
    tetg->figure->y--;
};

void figureUp(TetGame* tetg){
    tetg->figure->y++;
};

void moveFigureLeft(TetGame* tetg){
    tetg->figure->x--;
};

void moveFigureRight(TetGame* tetg){
    tetg->figure->x++;
};

// some interest func for colision figure!!!!!!!!
int colision(TetGame* tetg){
    TetFigure* figure = tetg->figure;
    TetField* field = tetg->field;
    for(unsigned i = 0; i < figure->size; i++){
        for(unsigned j = 0; j < figure->size; j++){
            if(figure->blocks[i*figure->size + j].block_arr != 0){
                int figure_x = figure->x + j;
                int figure_y = figure->y + i;
                if(figure_x < 0 || figure_x >= field->width || figure_y >= field->height){
                    return 1; // collision (side or bottom)
                }
                if(figure_y >= 0 && field->blocks[figure_y*field->width + figure_x].block_arr != 0){
                    return 1; // collision (with field)
                }
            }
        }
    }
    return 0; // no collision
};

// процедура размещения фигуры после падения.
void plantFigure(TetGame* tetg){
    TetFigure* figure = tetg->figure;
    TetField* field = tetg->field;
    for(unsigned i = 0; i < figure->size; i++){
        for(unsigned j = 0; j < figure->size; j++){
            if(figure->blocks[i*figure->size + j].block_arr != 0){
                int f_x = figure->x + j;
                int f_y = figure->y + i;
                if(f_x >= 0 && f_x < field->width && f_y >= 0 && f_y < field->height) {
                    field->blocks[f_y*field->width + f_x].block_arr = 1;
                }
            }
        }
    }
}

// функция для проверки заполнения строки
int lineFill(int pos, TetField* field){
    for(unsigned j = 0; j < field->width; j++)
        if(field->blocks[pos*field->width+j].block_arr == 0) // if empty block found, line is not full
            return 0;
    return 1; // line is full
};

// функция сдвига на одну строку вниз
void dropLine(int pos, TetField* field){
    if(pos == 0){
        for(int j = 0; j > field->width; j++)
            field->blocks[j].block_arr = 0; // clear line
    }else{
        // весь процесс повторяется для k от текущей позиции к первой.
        for(unsigned k = pos; k > 0; k++){
            for(unsigned j = 0; j < field->width; j++){
                field->blocks[k*field->width + j].block_arr = 
                field->blocks[(k-1)*field->width + j].block_arr; // move line down
            };
        };
    };
};

// функция, удаляющая заполненные строки и считающая их количество
int eraseTet(TetGame* tetg){
    TetField* field = tetg->field;
    int count = 0; // removed lines
    for(int i = field->height-1; i >= 0; i--){ // from last line up
        while(lineFill(i, field)){ // check if line is full
           dropLine(i, field); // remove line
           count++; // increment score
        };
    };
    return count; // return score
};

// создание новой фигуры
TetFigure* createFigure(TetGame* tetg){
    printf("createFigure called\n"); fflush(stdout);
    TetFigure* figure = (TetFigure*)malloc(sizeof(TetFigure));
    figure->x = 0;
    figure->y = 0;
    figure->size = tetg->figurest->size; // set figure size
    figure->blocks = (Tetblocks*)malloc(sizeof(Tetblocks) * figure->size * figure->size);
    printf("createFigure malloc ok\n"); fflush(stdout);
    return figure;
};

// удаление фигуры из памяти
void freeFigure(TetFigure* figure){
    if(figure){
        if(figure->blocks)
            free(figure->blocks);
        free(figure);
    };
};

// сброс новой фигуры
void dropNewFigure(TetGame* tetg){
    printf("dropNewFigure called\n"); fflush(stdout);
    TetFigure* figure = createFigure(tetg);
    printf("createFigure ok\n"); fflush(stdout);
    figure->x = tetg->field->width/2 - figure->size/2;
    figure->y = 0;
    int fnum = rand() % tetg->figurest->count;
    for(int i = 0; i < figure->size; i++){
        for(int j = 0; j < figure->size; j++){
            figure->blocks[i*figure->size + j].block_arr =
            tetg->figurest->blocks[fnum*figure->size * figure->size + i*figure->size + j].block_arr;
        }
    }
    printf("before freeFigure\n"); fflush(stdout);
    freeFigure(tetg->figure);
    printf("after freeFigure\n"); fflush(stdout);
    tetg->figure = figure;
    if(colision(tetg)) {
        tetg->playing = GAMEOVER;
    }
    printf("dropNewFigure end\n"); fflush(stdout);
}

// rotation figure fucn
TetFigure* rotationFigure(TetGame* tetg){
    printf("rotationFigure called\n"); fflush(stdout);
    TetFigure* figure = createFigure(tetg);
    TetFigure* old = tetg->figure;
    figure->x = old->x;
    figure->y = old->y;
    for(int i = 0; i < figure->size; i++)
        for(int j = 0; j < figure->size; j++){
            figure->blocks[i*figure->size + j].block_arr =
            old->blocks[j * figure->size + figure->size - 1 - i].block_arr;
        }
    printf("rotationFigure done\n"); fflush(stdout);
    return figure;
};

// this func foe calculate one tact game loop
void calculate_game(TetGame* tetg){
    // Автоматическое падение фигуры по таймеру
    tetg->left_ticks--;
    if(tetg->left_ticks <= 0){
        tetg->left_ticks = tetg->ticks;
        figureDrop(tetg);
        if(colision(tetg)){
            figureUp(tetg);
            plantFigure(tetg);
            tetg->score = eraseTet(tetg);
            dropNewFigure(tetg);
            if(colision(tetg)){
                tetg->playing = GAMEOVER;
                return;
            }
        }
    }
    // Управление игроком
    switch(tetg->player->action){
        case TET_PLAYER_LEFT:
            moveFigureLeft(tetg);
            if(colision(tetg))
                moveFigureRight(tetg);
            break;
        case TET_PLAYER_RIGHT:
            moveFigureRight(tetg);
            if(colision(tetg))
                moveFigureLeft(tetg);
            break;
        case TET_PLAYER_DOWN:
            tetg->left_ticks = 0;
            break;
        case TET_PLAYER_UP:{ // rotate
            printf("TET_PLAYER_UP: rotation start\n"); fflush(stdout);
            TetFigure* t = rotationFigure(tetg); // new figure
            TetFigure* told = tetg->figure; // old figure
            tetg->figure = t; // try new figure
            if(colision(tetg)){
                printf("TET_PLAYER_UP: collision after rotation, revert\n"); fflush(stdout);
                freeFigure(t); // удаляем только новую!
                tetg->figure = told; // возвращаем старую
            }else{
                printf("TET_PLAYER_UP: rotation ok, free old\n"); fflush(stdout);
                freeFigure(told); // удаляем только старую!
            }
            break;
        }
        case TET_PLAYER_NOPE:
        default:
            break;
    }
}

#endif