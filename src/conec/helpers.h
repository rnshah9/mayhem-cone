/** Compiler Helpers: Memory, File i/o, Symbols, AST
 * @file
 *
 * This source file is part of the Cone Programming Language C compiler
 * See Copyright Notice in conec.h
*/

#ifndef helpers_h
#define helpers_h

#include <stdio.h>

/* *****************************************************
 * Memory - memory.c
 * *****************************************************/

// Allocate memory for a block, aligned to a 16-byte boundary
void *memAllocBlk(size_t size);

// Allocate memory for a string and copy contents over, if not NULL
// Allocates extra byte for string-ending 0, appending it to copied string
char *memAllocStr(char *str, size_t size);

/* *****************************************************
 * File i/o - fileio.c
 * *****************************************************/

// Load a file into an allocated string, return pointer or NULL if not found
char *fileLoad(char *fn);

#endif