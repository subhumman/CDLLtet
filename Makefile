TARGET = CTetris.exe
CC = clang
SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Основное правило
$(TARGET): $(OBJ)
	$(CC) $^ -o $@

# Правило компиляции
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@

# Создание папки для объектных файлов
$(OBJ_DIR):
	@mkdir "$@"

# Очистка
clean:
	@if exist "$(TARGET)" del /Q "$(TARGET)"
	@if exist "$(OBJ_DIR)" rmdir /S /Q "$(OBJ_DIR)"

.PHONY: clean