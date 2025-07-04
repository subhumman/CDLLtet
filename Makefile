TARGET = tetris.exe
CC = clang
SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# Main rule
$(TARGET): $(OBJ)
	$(CC) $^ -o $@

# Compilation rule
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@

# Create folder for object files
$(OBJ_DIR):
	@mkdir "$@"

# Cleaning
clean:
	@if exist "$(TARGET)" del /Q "$(TARGET)"
	@if exist "$(OBJ_DIR)" rmdir /S /Q "$(OBJ_DIR)"

.PHONY: clean