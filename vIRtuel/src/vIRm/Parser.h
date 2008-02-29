/**
 * @file Parser.h
 * 
 * author MIETTE Tom
 * author MOURET Sebastien
 * 
 * Defines some function to parse the shell's command line.
 * 
 */
#ifndef PARSER_H_
#define PARSER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Common.h"
#include "Util.h"

extern int debug;

/**
 * General function permiting to split the shell buffer and fill commad structure.
 * @param command * cmd structure to fill.
 */
void splitCommand(command * cmd);

#endif /*PARSER_H_*/
