#===========================================================
#Project name : vIRtuel machin
#Version : 1.0
#Authors : Tom Miette & Sebastien Mouret
#-----------------------------------------------------------
#makefile to create calc.so library
#Created February 28, 2008 by Tom Miette & Sebastien Mouret
#===========================================================

# Source files
export SRC = $(SRC_DIR)calc.c
# Object files
OBJ = $(SRC:.c=.o)
# Library
LIB = $(LIB_DIR)calc.so

all: $(LIB)

# Targets to building .so libraries
$(LIB): $(OBJ)
	@echo ----------------------------
	@echo Building library $(LIB)
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

# Target to clean temporary files and libraries
clean:
	@echo ----------------------------
	@echo Removing files
	@echo ----------------------------
	rm -f $(SRC_DIR)*.o $(SRC_DIR)*~ $(LIB)
	@echo ----------------------------
