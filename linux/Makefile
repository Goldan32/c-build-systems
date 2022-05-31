SRC_DIR  := sources
OBJ_DIR  := build/obj
BIN_DIR  := build
INC_DIR  := include
LIB_DIR  := libraries
EXE_NAME := helloworld

EXE := $(BIN_DIR)/$(EXE_NAME)
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

RMDIR := rm -rf

CC       := gcc
CPPFLAGS := -I$(INC_DIR) -MMD -MP
CFLAGS   := -Wall -Werror
LDFLAGS  := -L$(LIB_DIR)
LDLIBS   := -lm -static $(patsubst $(LIB_DIR)/lib*.a, -l$(LIB_DIR)/*, $(wildcard $(LIB_DIR)/lib*.a))

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	$(RMDIR) $(OBJ_DIR) $(BIN_DIR)

-include $(OBJ:.o=.d)
