import ctypes
import os
from tkinter import *

FIELD_WIDTH = 10  #Field parameter
FIELD_HEIGHT = 20
CELL_SIZE = 48
FIGURE_SIZE = 4

CUBE_COLOR = '#111111'  #black blocks
BG_COLOR = '#FFFFFF'    #white background
SCORE_COLOR = '#111111' #black score

DLL_PATH = os.path.abspath('tetris.dll')  #Load DLL
tetris = ctypes.CDLL(DLL_PATH)

# Function prototypes
tetris.tetris_init.restype = None  # void tetris_init()
tetris.tetris_step.argtypes = [ctypes.c_int]  # void tetris_step(int action)
tetris.tetris_step.restype = None
tetris.tetris_get_field.argtypes = [ctypes.POINTER(ctypes.c_int)]  # void tetris_get_field(int* out_array)
tetris.tetris_get_field.restype = None
tetris.tetris_get_figure.argtypes = [ctypes.POINTER(ctypes.c_int)]  # void tetris_get_figure(int* out_array)
tetris.tetris_get_figure.restype = None
tetris.tetris_get_gameover.restype = ctypes.c_int  # int tetris_get_gameover()
tetris.tetris_get_score.restype = ctypes.c_int     # int tetris_get_score()
tetris.tetris_tick.restype = None                  # void tetris_tick()

ACTION_NONE = 0  #Action codes (as in C)
ACTION_UP = 1
ACTION_DOWN = 2
ACTION_LEFT = 3
ACTION_RIGHT = 4

# Determine figure type by coordinates (for color)
def get_figure_type():
    return (tetris.tetris_get_score() + 1)

def c_init_game():
    tetris.tetris_init()

def c_step_game(action):
    tetris.tetris_step(action)

def c_get_field():
    arr = (ctypes.c_int * (FIELD_WIDTH * FIELD_HEIGHT))()
    tetris.tetris_get_field(arr)
    return [[arr[y*FIELD_WIDTH + x] for x in range(FIELD_WIDTH)] for y in range(FIELD_HEIGHT)]

def c_get_figure():
    arr = (ctypes.c_int * (FIGURE_SIZE * FIGURE_SIZE * 2))()
    tetris.tetris_get_figure(arr)
    coords = []
    for i in range(FIGURE_SIZE * FIGURE_SIZE):
        x = arr[i*2]
        y = arr[i*2+1]
        if x >= 0 and y >= 0:
            coords.append((x, y))
    return coords

def c_get_gameover():
    return tetris.tetris_get_gameover()

def c_get_score():
    return tetris.tetris_get_score()

def draw_field(canvas, field, figure):
    canvas.delete('all')
    # Background
    canvas.create_rectangle(0, 0, FIELD_WIDTH*CELL_SIZE, FIELD_HEIGHT*CELL_SIZE, fill=BG_COLOR, outline='')
    # Field
    for y in range(FIELD_HEIGHT):
        for x in range(FIELD_WIDTH):
            if field[y][x]:
                canvas.create_rectangle(x*CELL_SIZE+3, y*CELL_SIZE+3, (x+1)*CELL_SIZE-3, (y+1)*CELL_SIZE-3, fill=CUBE_COLOR, outline='')
    # Figure
    for (fx, fy) in figure:
        if 0 <= fx < FIELD_WIDTH and 0 <= fy < FIELD_HEIGHT:
            canvas.create_rectangle(fx*CELL_SIZE+3, fy*CELL_SIZE+3, (fx+1)*CELL_SIZE-3, (fy+1)*CELL_SIZE-3, fill=CUBE_COLOR, outline='')
    # Score
    score = c_get_score()
    canvas.create_text(FIELD_WIDTH*CELL_SIZE//2, 32, text=f'Score: {score}', fill=SCORE_COLOR, font=('Segoe UI', 28, 'bold'))

def on_key(event):
    key = event.keysym
    if key == 'Left':
        c_step_game(ACTION_LEFT)
    elif key == 'Right':
        c_step_game(ACTION_RIGHT)
    elif key == 'Down':
        c_step_game(ACTION_DOWN)
    elif key == 'Up':
        c_step_game(ACTION_UP)
    else:
        c_step_game(ACTION_NONE)
    update()

restart_btn = None

def show_restart():
    global restart_btn
    if restart_btn is None:
        restart_btn = Button(root, text='Restart', font=('Segoe UI', 20, 'bold'), bg='#F4F4F4', fg='#111', bd=0, highlightthickness=0, command=restart_game)
        restart_btn.place(x=FIELD_WIDTH*CELL_SIZE//2-80, y=FIELD_HEIGHT*CELL_SIZE//2+60, width=160, height=48)

def hide_restart():
    global restart_btn
    if restart_btn is not None:
        restart_btn.destroy()
        restart_btn = None

def restart_game():
    c_init_game()
    hide_restart()
    update()

def tick():
    tetris.tetris_tick()
    update()
    root.after(400, tick)  # only from here the next tick is started

def update():
    field = c_get_field()
    figure = c_get_figure()
    draw_field(canvas, field, figure)
    if not c_get_gameover():
        hide_restart()
    else:
        canvas.create_text(FIELD_WIDTH*CELL_SIZE//2, FIELD_HEIGHT*CELL_SIZE//2, text='GAME OVER', fill=CUBE_COLOR, font=('Segoe UI', 48, 'bold'))
        show_restart()

root = Tk()
root.title('Tetris (C backend + Tkinter GUI)')
canvas = Canvas(root, width=FIELD_WIDTH*CELL_SIZE, height=FIELD_HEIGHT*CELL_SIZE, bg=BG_COLOR, highlightthickness=0)
canvas.pack()
root.resizable(False, False)
root.bind('<Key>', on_key)

c_init_game()
update()
tick()
root.mainloop() 
