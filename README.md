## The makefile in this project is useless. Initially, the game was written in pure C, but I wanted to test working with dlls and combine it with Python. ##

---

## Features
- **True Classic Tetris Mechanics** — pure, fair, and bug-free gameplay
- **Constant Fall Speed** — blocks always fall at a steady, classic pace
- **Minimalist Design** — clean white background, bold black blocks, elegant score display
- **Instant Controls** — arrow keys for movement and rotation, no lag, no acceleration hacks
- **Restart Button** — quick restart for endless fun
- **Cross-Platform** — runs on Windows (and easily portable to Linux/macOS)
- **Hybrid Architecture** — C for performance, Python for beautiful UI

---

## Screenshots
> _Add your screenshots here!_
![Tetris Gameplay](screenshots/tetris_game.png)
---

## How to Build & Run

### 1. **Build the C backend (DLL)**
```sh
clang -shared -o tetris.dll src/tetris_api.c
```

### 2. **Run the Python GUI**
```sh
python src/tetris_gui.py
```

### 3. **Build a Standalone EXE (optional)**
```sh
pip install pyinstaller
pyinstaller --onefile --noconsole --add-binary "tetris.dll;." src/tetris_gui.py
```
- The EXE will appear in the `dist/` folder. Place `tetris.dll` next to it if needed.

---

## Controls
- **Left/Right Arrow** — Move block left/right
- **Up Arrow** — Rotate block
- **Down Arrow** — Move block down by one cell
- **Restart** — Click the Restart button after Game Over

---

## Architecture
- **C Backend** — Handles all game logic, collision, scoring, and field management for maximum speed and reliability.
- **Python GUI** — Renders the game in a beautiful, minimalist window. Handles user input and displays score/game state.
- **Communication** — Python calls C functions via ctypes for seamless, real-time updates.

---

## Why This Project Stands Out
- **Professional code structure** — clean, modular, and easy to extend
- **No hacks, no bugs, no flicker** — just pure, smooth gameplay
- **Designed for both learning and fun** — perfect for code review, portfolio, or just playing
- **A showcase of hybrid C/Python engineering**

---

##  Credits
**Author:** Subhuman/Tilda/Denis
**TG:** @TildasSkinPeeler
**email:** denis02012000@hotmail.com
