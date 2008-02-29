#===========================================================
#Project name : vIRtuel machin
#Version : 1.0
#Authors : Tom Miette & Sebastien Mouret
#-----------------------------------------------------------
#makefile to create oreader executable
#Created February 28, 2008 by Tom Miette & Sebastien Mouret
#===========================================================

# Source file directory
SRC = $(SRC_DIR)Main.c\
	$(GEN_SRC)
# Object files
OBJ = $(SRC:.c=.o)
# Executable
EXEC = $(BIN_DIR)oreader

all = $(EXEC)

# Target to create executable
$(EXEC): $(OBJ)
	@echo ----------------------------
	@echo Build  $(EXEC)
	@echo ----------------------------
	$(CC) $(CFLAGS) -o$(EXEC) $(OBJ)
	@echo ----------------------------

# Target to compile source files
%.o: %.c
	@echo ----------------------------
	@echo Compiling $<
	@echo ----------------------------
	$(CC) -c $(CFLAGS) $(INCLUDE_DIR) -o $@ $<
	@echo ----------------------------

.PHONY: clean mrproper

# Target to clean object and temporary files
clean:
	@echo ----------------------------
	@echo Removing files
	@echo ----------------------------
	rm -f $(SRC_DIR)*.o $(SRC_DIR)*~
	@echo ----------------------------

# Target to clean executable
mrproper: clean
	@echo ----------------------------
	@echo Removing executable file
	@echo ----------------------------
	rm -f $(EXEC)
	@echo ----------------------------
