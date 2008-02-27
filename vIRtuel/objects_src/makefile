# Makefiles
MAKEFILE = makefile Makefile.libcsv Makefile.libods
# Répertoire contenant les fichiers sources
REP_SRC = src/
# Répertoire contenant les fichiers sources de l'etl
REP_SRC_ETL = $(REP_SRC)etl/
# Répertoire contenant les fichiers sources générique
REP_SRC_INCL = $(REP_SRC)include/
# Répertoire contenant l'exécutable
REP_BIN = bin/
# Répertoire contenant les fichiers root
REP_ROOT = root/
# Répertoire contenant les fichiers data
REP_DATA = data/
# Répertoire contenant les fichiers tests
REP_TESTS = tests/
# Répertoire contenant le rapport
REP_RAPP = rapport/
# Compilateur
CC = gcc
# Options de compilation
CFLAGS = -Wall -Werror -ansi
# Fichiers sources du projet
SRC = $(REP_SRC)main.c\
	$(REP_SRC_INCL)util.c\
	$(REP_SRC_ETL)iretlfactory.c\
	$(REP_SRC_ETL)iretlopt.c\
	$(REP_SRC_ETL)iretlrules.c\
	$(REP_SRC_ETL)iretlinterface.c\
	$(REP_SRC_ETL)iretl.c
# Répertoire d'inclusions
REP_INCLUDE = -I$(REP_SRC) -I$(REP_SRC_ETL) -I$(REP_SRC_INCL)
# Fichiers objets
OBJ = $(SRC:.c=.o)
# Exécutable
EXEC = $(REP_BIN)iretl
# Archive .tar.gz
TAR = MIETTE-MOURET.tar.gz



# Cible par défaut
all = $(EXEC)

# Cible pour construire l'exécutable
$(EXEC): $(OBJ)
	@echo ----------------------------
	@echo Link  $(EXEC)
	@echo ----------------------------
	$(CC) $(CFLAGS) -o$(EXEC) $(OBJ) -ldl -rdynamic 
	@echo ----------------------------

# Cible pour compiler les fichier sources
%.o: %.c
	@echo ----------------------------
	@echo Compiling $<
	@echo ----------------------------
	$(CC) -c $(CFLAGS) $(REP_INCLUDE) -o $@ $<
	@echo ----------------------------

# Cible dont les dépendances seront systématiquement construites
.PHONY: clean mrproper

# Cible pour la suppression des fichiers .o et ~
clean:
	@echo ----------------------------
	@echo Removing object files
	@echo ----------------------------
	rm -rf $(OBJ)
	@echo ----------------------------
	@echo Removing backup files
	@echo ----------------------------
	rm -rf $(REP_SRC)*~ $(REP_SRC_ETL)*~ $(REP_SRC_INCL)*~ $(REP_DATA)*~ $(REP_TESTS)*~ $(REP_ROOT)*~
	@echo ----------------------------

# Cible pour la suppression des fichiers .o et ~ et de l'exécutable
mrproper: clean
	@echo ----------------------------
	@echo Removing executable file
	@echo ----------------------------
	rm -rf $(EXEC)
	@echo ----------------------------
	@echo Removing archive file
	@echo ----------------------------
	rm -rf $(TAR)
	@echo ----------------------------

# Cible pour la compression du projet (répertoire courant)
tar: $(EXEC) clean
	@echo ----------------------------
	@echo Compressing project
	@echo ----------------------------
	tar -cf $(TAR) $(REP_SRC) $(REP_BIN) $(REP_DATA) $(REP_ROOT) $(REP_TESTS) $(REP_RAPP) $(MAKEFILE)
	@echo ----------------------------
