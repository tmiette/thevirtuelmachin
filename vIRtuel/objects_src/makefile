#===========================================================
#Project name : vIRtuel machin
#Version : 1.0
#Authors : Tom Miette & Sebastien Mouret
#-----------------------------------------------------------
#makefile to create .so libraries
#Created February 28, 2008 by Tom Miette & Sebastien Mouret
#===========================================================

# Source directory
export SRC_DIR = ./
# Include directories
export INCLUDE_DIR = -I$(SRC_DIR)
# Libraries directory
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

# Targets to building .so libraries
libs:
	@(cd $(CALC_DIR) && $(MAKE))
	@(cd $(INT2STRING_DIR) && $(MAKE))
	@(cd $(STRING2INT_DIR) && $(MAKE))
	
.PHONY: clean

# Target to clean temporary files and libraries
clean:
	@(cd $(CALC_DIR) && $(MAKE) $@)
	@(cd $(INT2STRING_DIR) && $(MAKE) $@)
	@(cd $(STRING2INT_DIR) && $(MAKE) $@)
	@echo ----------------------------
	@echo Removing files
	@echo ----------------------------
	rm -f $(SRC_DIR)*.o $(SRC_DIR)*~
	@echo ----------------------------
	