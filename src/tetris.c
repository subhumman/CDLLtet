#include "tetris.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef TETRIS_C
#define TETRIS_C

/**
 * Create a new Tetris field.
 * @param width Field width
 * @param height Field height
 * @return Pointer to TetField
 */
TetField* createField(int width, int height){
    TetField* field = (TetField*)malloc(sizeof(TetField));
    field->width = width;
    field->height = height;
    field->blocks = (Tetblocks*)malloc(sizeof(Tetblocks)*width*height);
    for(unsigned i = 0; i < width*height; i++)
        field->blocks[i].block_arr = 0; // empty block
    return field;
};

/* Free memory allocated for the field.*/
void freeFieldMemory(TetField* field){
    if(field){
        if(field->blocks)
            free(field->blocks);
        free(field);
    };
};

/* Create figure templates structure. */
TetFiguresT* createFiguresT(int count, int figure_size, Tetblocks* figureT){
    TetFiguresT* figT = (TetFiguresT*)malloc(sizeof(TetFiguresT));
    figT->blocks = figureT;
    figT->count = count;
    figT->size = figure_size;
    return figT;
};

/* Free memory for figure templates structure.*/
void freeFigTemplateMalloc(TetFiguresT* figure){
    if(figure)
        free(figure);
};

/* Initialize a new Tetris game.*/
TetGame* StartGame(int field_w, int field_h, int figure_size, int count, Tetblocks* figure_templates){
    TetGame* tetg = (TetGame*)malloc(sizeof(TetGame));
    tetg->field = createField(field_w, field_h);
    tetg->figurest = createFiguresT(count, figure_size, figure_templates);
    tetg->ticks = TET_TICKS_START;
    tetg->left_ticks = TET_TICKS_START;
    tetg->playing = PLAYING;
    tetg->score = 0;
    return tetg;
};

/* Free all memory for the game.*/
void freeStartGame(TetGame* game){
    if(game){
        freeFieldMemory(game->field);
        freeFigTemplateMalloc(game->figurest);
        free(game);
    };
};

/* Move the current figure down by one cell.*/
void figureDrop(TetGame* tetg){
    tetg->figure->y--;
};

/* Move the current figure up by one cell (undo drop).*/
void figureUp(TetGame* tetg){
    tetg->figure->y++;
};

/* Move the current figure left by one cell.*/
void moveFigureLeft(TetGame* tetg){
    tetg->figure->x--;
};

/* Move the current figure right by one cell.*/
void moveFigureRight(TetGame* tetg){
    tetg->figure->x++;
};

/**
 * Check for collision between the figure and the field or boundaries.
 * @return 1 if collision, 0 otherwise
 */
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

/* Place the current figure onto the field after it lands.*/
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

/**
 * Check if a line is completely filled.
 * @return 1 if full, 0 otherwise
 */
int lineFill(int pos, TetField* field){
    for(unsigned j = 0; j < field->width; j++)
        if(field->blocks[pos*field->width+j].block_arr == 0)
            return 0;
    return 1;
};

/* Drop all lines above the given position by one */
void dropLine(int pos, TetField* field){
    if(pos == 0){
        for(int j = 0; j < field->width; j++)
            field->blocks[j].block_arr = 0;
    }else{
        for(unsigned k = pos; k > 0; k--){
            for(unsigned j = 0; j < field->width; j++){
                field->blocks[k*field->width + j].block_arr = 
                field->blocks[(k-1)*field->width + j].block_arr;
            };
        };
    };
};

/* Remove filled lines and return the number of lines removed.*/
int eraseTet(TetGame* tetg){
    TetField* field = tetg->field;
    int count = 0;
    for(int i = field->height-1; i >= 0; i--){
        while(lineFill(i, field)){
           dropLine(i, field);
           count++;
        };
    };
    return count;
};

/* Create a new figure for the game.*/
TetFigure* createFigure(TetGame* tetg){
    TetFigure* figure = (TetFigure*)malloc(sizeof(TetFigure));
    figure->x = 0;
    figure->y = 0;
    figure->size = tetg->figurest->size;
    figure->blocks = (Tetblocks*)malloc(sizeof(Tetblocks) * figure->size * figure->size);
    return figure;
};

/* Free memory for a figure.*/
void freeFigure(TetFigure* figure){
    if(figure){
        if(figure->blocks)
            free(figure->blocks);
        free(figure);
    };
};

/* Drop a new figure into the game. Ends game if placement collides.*/
void dropNewFigure(TetGame* tetg){
    TetFigure* figure = createFigure(tetg);
    figure->x = tetg->field->width/2 - figure->size/2;
    figure->y = 0;
    int fnum = rand() % tetg->figurest->count;
    for(int i = 0; i < figure->size; i++){
        for(int j = 0; j < figure->size; j++){
            figure->blocks[i*figure->size + j].block_arr =
            tetg->figurest->blocks[fnum*figure->size * figure->size + i*figure->size + j].block_arr;
        }
    }
    freeFigure(tetg->figure);
    tetg->figure = figure;
    if(colision(tetg)) {
        tetg->playing = GAMEOVER;
    }
}

/**
 * Rotate the current figure clockwise.
 * @return Pointer to new rotated figure
 */
TetFigure* rotationFigure(TetGame* tetg){
    TetFigure* figure = createFigure(tetg);
    TetFigure* old = tetg->figure;
    figure->x = old->x;
    figure->y = old->y;
    for(int i = 0; i < figure->size; i++)
        for(int j = 0; j < figure->size; j++){
            figure->blocks[i*figure->size + j].block_arr =
            old->blocks[j * figure->size + figure->size - 1 - i].block_arr;
        }
    return figure;
};

/* Perform one tick of the game loop: move, drop, erase, and spawn figures */
void calculate_game(TetGame* tetg){
    // Automatic figure drop by timer
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
    // Handle player input
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
            figureDrop(tetg);
            if(colision(tetg))
                figureUp(tetg);
            break;
        case TET_PLAYER_UP: {
            TetFigure* rotated = rotationFigure(tetg);
            TetFigure* old = tetg->figure;
            tetg->figure = rotated;
            if(colision(tetg)){
                freeFigure(rotated);
                tetg->figure = old;
            } else {
                freeFigure(old);
            }
            break;
        }
        default:
            break;
    }
}

#endif