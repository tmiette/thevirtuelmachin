# Répertoire contenant les fichiers sources
REP_SRC = *
# Répertoire contenant les fichiers sources générique
REP_SRC_INCL = ../include/
# Répertoire contenant l'exécutable
REP_BIN = ../../bin/
# Compilateur
CC = gcc
# Options de compilation
CFLAGS = -Wall -ansi
# Fichiers sources du projet
SRC = $(REP_SRC)Main.c\
	$(REP_SRC_INCL)Util.c
# Répertoire d'inclusions
REP_INCLUDE = -I$(REP_SRC) -I$(REP_SRC_INCL)
# Fichiers objets
OBJ = $(SRC:.c=.o)
# Exécutable
EXEC = $(REP_BIN)oreader



# Cible par défaut
all = $(EXEC)

# Cible pour construire l'exécutable
$(EXEC): $(OBJ)
	@echo ----------------------------
	@echo Link  $(EXEC)
	@echo ----------------------------
	$(CC) $(CFLAGS) -o$(EXEC) $(OBJ)
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
	rm -rf $(REP_SRC)*~
	@echo ----------------------------

# Cible pour la suppression des fichiers .o et ~ et de l'exécutable
mrproper: clean
	@echo ----------------------------
	@echo Removing executable file
	@echo ----------------------------
	rm -rf $(EXEC)
