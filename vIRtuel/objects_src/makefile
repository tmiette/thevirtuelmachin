# Source directory
SRC_DIR = ./
# Librairies directory
LIB_DIR = ../objects/
# Compiler
CC = gcc
# Compilation options
CFLAGS = -Wall -ansi -fPIC
# Include directories
INCLUDE_DIR = -I$(SRC_DIR)
# Source files
SRC_CALC = $(SRC_DIR)calc.c
SRC_INT2S = $(SRC_DIR)int2string.c
SRC_S2INT = $(SRC_DIR)string2int.c
# Object files
OBJ_CALC = $(SRC_CALC:.c=.o)
OBJ_INT2S = $(SRC_INT2S:.c=.o)
OBJ_S2INT = $(SRC_S2INT:.c=.o)
# Librairy
LIB_CALC = $(LIB_DIR)calc.so
LIB_INT2S = $(LIB_DIR)int2string.so
LIB_S2INT = $(LIB_DIR)string2int.so


all: $(LIB_CALC) $(LIB_INT2S) $(LIB_S2INT)

# Targets to building .so librairies
$(LIB_CALC): $(OBJ_CALC)
	@echo ----------------------------
	@echo Building librairy $(LIB_CALC)
	@echo ----------------------------
	ld -shared $(OBJ_CALC) -o $(LIB_CALC)
	@echo ----------------------------

$(LIB_INT2S): $(OBJ_INT2S)
	@echo ----------------------------
	@echo Building librairy $(LIB_INT2S)
	@echo ----------------------------
	ld -shared $(OBJ_INT2S) -o $(LIB_INT2S)
	@echo ----------------------------
	
$(LIB_S2INT): $(OBJ_S2INT)
	@echo ----------------------------
	@echo Building librairy $(LIB_S2INT)
	@echo ----------------------------
	ld -shared $(OBJ_S2INT) -o $(LIB_S2INT)
	@echo ----------------------------
	
# Target to compile each source file
%.o: %.c
	@echo ----------------------------
	@echo Compiling $<
	@echo ----------------------------
	$(CC) -c $(CFLAGS) $(INCLUDE_DIR) -o $@ $<
	@echo ----------------------------

# Target to clean temporary files and librairies
clean:
	@echo ----------------------------
	@echo Removing files
	@echo ----------------------------
	rm -f $(SRC_DIR)*.o $(SRC_DIR)*~ $(LIB_CALC) $(LIB_INT2S) $(LIB_S2INT)
	@echo ----------------------------
