# makefile : calc

# Source files
export SRC = $(SRC_DIR)calc.c
# Object files
OBJ = $(SRC:.c=.o)
# Librairy
LIB = $(LIB_DIR)calc.so

all: $(LIB)

# Targets to building .so librairies
$(LIB): $(OBJ)
	@echo ----------------------------
	@echo Building librairy $(LIB)
	@echo ----------------------------
	ld -shared $(OBJ) -o $(LIB)
	@echo ----------------------------

# Target to compile each source file
%.o: %.c
	@echo ----------------------------
	@echo Compiling $<
	@echo ----------------------------
	$(CC) -c $(CFLAGS) $(INCLUDE_DIR) -o $@ $<
	@echo ----------------------------

.PHONY: clean

# Target to clean temporary files and librairies
clean:
	@echo ----------------------------
	@echo Removing files
	@echo ----------------------------
	rm -f $(SRC_DIR)*.o $(SRC_DIR)*~ $(LIB)
	@echo ----------------------------
