## скрин
<img width="347" height="1071" alt="image" src="https://github.com/user-attachments/assets/78e1f815-a9c3-4a38-ae21-34813c09c72c" />

---

## как запустить

### 1. забилдить дллки
```sh
clang -shared -o tetris.dll src/tetris_api.c
```

### 2. запустить питоновские скрипты для gui 
```sh
python src/tetris_gui.py
```

### 3. собрать exe
```sh
pip install pyinstaller
pyinstaller --onefile --noconsole --add-binary "tetris.dll;." src/tetris_gui.py
```
- exe будет находиться в `dist/`.

---

## Архитектура
- **C бэк** — логика вся
- **Python GUI** — отображение
---



