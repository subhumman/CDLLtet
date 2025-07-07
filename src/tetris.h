#ifndef TETRIS_H
#define TETRIS_H

#define TET_TICKS_START 30 // ticks between block drops

// Player action constants
enum {
    TET_PLAYER_NOPE = 0,
    TET_PLAYER_UP,
    TET_PLAYER_DOWN,
    TET_PLAYER_LEFT,
    TET_PLAYER_RIGHT
};

// Game state constants
enum{
    GAMEOVER = 0,
    PLAYING
};

/* Player action structure*/
typedef struct TetPlayer{
    int action; // current action
} TetPlayer;

/* Field block structure */
typedef struct Tetblocks{
    int block_arr; // block value (0 = empty, 1 = filled)
} Tetblocks;

/* Tetris field structure*/
typedef struct TetField{
    int width; // field width
    int height; // field height
    Tetblocks* blocks; // pointer to field blocks
} TetField;

/* Tetris figure structure*/
typedef struct TetFigure{
    int x; // x position
    int y; // y position
    int size; // figure size
    Tetblocks* blocks; // pointer to figure blocks
} TetFigure;

/* Figure templates structure*/
typedef struct TetFiguresT{
    int count; // number of templates
    int size; // template size
    Tetblocks* blocks; // pointer to template blocks
} TetFiguresT;

/* Main game structure*/
typedef struct TetGame{
    TetField* field; // pointer to field
    TetFigure* figure; // pointer to current figure
    TetFiguresT* figurest; // pointer to templates
    TetPlayer* player; // pointer to player
    int score; // current score
    int ticks; // ticks per drop
    int left_ticks; // ticks left until next drop
    int playing; // game state
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