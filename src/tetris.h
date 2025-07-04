#ifndef TETRIS_H
#define TETRIS_H

#define TET_TICKS_START 30 // ticks between block drops

enum { // consts of player actions
    TET_PLAYER_NOPE = 0,
    TET_PLAYER_UP,
    TET_PLAYER_DOWN,
    TET_PLAYER_LEFT,
    TET_PLAYER_RIGHT
};

enum{ // consts of game time
    GAMEOVER = 0,
    PLAYING
};

typedef struct TetPlayer{
    int action;
} TetPlayer;

typedef struct Tetblocks{
    int block_arr; // field block
} Tetblocks;

typedef struct TetField{
    int width; // field width
    int height; // field height
    Tetblocks* blocks; // field blocks
} TetField;

typedef struct TetFigure{
    int x; // figure x
    int y; // figure y
    int size; // figure size
    Tetblocks* blocks; // figure blocks
} TetFigure;

typedef struct TetFiguresT{ // figure templates
    int count; // template count
    int size; // template size
    Tetblocks* blocks; // template blocks
} TetFiguresT;

typedef struct TetGame{
    TetField* field; // field
    TetFigure* figure; // figure
    TetFiguresT* figurest; // templates
    TetPlayer* player; // set by user
    int score;
    int ticks;
    int left_ticks;
    int playing;
} TetGame;

extern Tetblocks figure_template[];

// Function prototypes

TetGame* StartGame(int field_w, int field_h, int figure_size, int count, Tetblocks* figure_template);
void calculate_game(TetGame* tetg);
TetField* createField(int width, int height);
void freeFieldMemory(TetField* field);
TetFiguresT* createFiguresT(int count, int figure_size, Tetblocks* figureT);
void freeFigTemplateMalloc(TetFiguresT* figure);
TetGame* StartGame(int field_w, int field_h, int figure_size, int count, Tetblocks* figure_templates);
void freeStartGame(TetGame* game);
void figureDrop(TetGame* tetg);
void figureUp(TetGame* tetg);
void moveFigureLeft(TetGame* tetg);
void moveFigureRight(TetGame* tetg);
int colision(TetGame* tetg);
void plantFigure(TetGame* tetg);
int lineFill(int pos, TetField* field);
void dropLine(int pos, TetField* field);
int eraseTet(TetGame* tetg);
TetFigure* createFigure(TetGame* tetg);
void freeFigure(TetFigure* figure);
void dropNewFigure(TetGame* tetg);
TetFigure* rotationFigure(TetGame* tetg);
void calculate_game(TetGame* tetg);

#endif