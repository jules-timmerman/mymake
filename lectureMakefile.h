#ifndef LECTUREMAKEFILE_H
#define LECTUREMAKEFILE_H

#include "listeRegles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

listeRegles_t* makefile2list(FILE* makefile);

#endif