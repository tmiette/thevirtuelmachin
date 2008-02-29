#===========================================================
#Project name : vIRtuel machin
#Version : 1.0
#Authors : Tom Miette & Sebastien Mouret
#-----------------------------------------------------------
#makefile to create executables
#Created February 28, 2008 by Tom Miette & Sebastien Mouret
#===========================================================

# Source directory
SRC_DIR = ./
# Generic include directory
INCLUDE_GEN_DIR = ../include/
# Include directories
export INCLUDE_DIR = -I$(SRC_DIR) -I$(INCLUDE_GEN_DIR)
# Generic source files
export GEN_SRC = $(INCLUDE_GEN_DIR)Util.c
# Excutables directory
export BIN_DIR = ../../bin/
# Compiler
export CC = gcc
# Compilation options
export CFLAGS = -Wall
# Sub-directories
VIRM_DIR = ./vIRm
OREADER_DIR = ./oreader
LAUNCH_DIR = ./launch

all: exec

# Target to create executables
exec:
	@(cd $(VIRM_DIR) && $(MAKE))
	@(cd $(OREADER_DIR) && $(MAKE))
	@(cd $(LAUNCH_DIR) && $(MAKE))
	
.PHONY : clean mrproper

# Target to clean object and temporary files
clean:
	@(cd $(VIRM_DIR) && $(MAKE) $@)
	@(cd $(OREADER_DIR) && $(MAKE) $@)
	@(cd $(LAUNCH_DIR) && $(MAKE) $@)
	@echo ----------------------------
	@echo Removing files
	@echo ----------------------------
	rm -f $(SRC_DIR)*.o $(SRC_DIR)*~
	@echo ----------------------------
	
# Target to clean executables
mrproper: clean
	@(cd $(VIRM_DIR) && $(MAKE) $@)
	@(cd $(OREADER_DIR) && $(MAKE) $@)
	@(cd $(LAUNCH_DIR) && $(MAKE) $@)
