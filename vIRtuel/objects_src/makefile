# Source directory
export SRC_DIR = ./
# Include directories
export INCLUDE_DIR = -I$(SRC_DIR)
# Librairies directory
export LIB_DIR = ../../objects/
# Compiler
export CC = gcc
# Compilation options
export CFLAGS = -Wall -ansi -fPIC
# Sub-directories
CALC_DIR = ./calc
INT2STRING_DIR = ./int2string
STRING2INT_DIR = ./string2int

all: libs

# Targets to building .so librairies
libs:
	@(cd $(CALC_DIR) && $(MAKE))
	@(cd $(INT2STRING_DIR) && $(MAKE))
	@(cd $(STRING2INT_DIR) && $(MAKE))
	
.PHONY: clean

# Target to clean temporary files and librairies
clean:
	@(cd $(CALC_DIR) && $(MAKE) $@)
	@(cd $(INT2STRING_DIR) && $(MAKE) $@)
	@(cd $(STRING2INT_DIR) && $(MAKE) $@)
	@echo ----------------------------
	@echo Removing files
	@echo ----------------------------
	rm -f $(SRC_DIR)*.o $(SRC_DIR)*~
	@echo ----------------------------
	