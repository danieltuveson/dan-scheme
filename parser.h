
#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "sexpression.h"
#include "utils.h"

struct SExpression *parse_sexpression(char buffer[BUFF_SIZE], int *start_index);
void enter_cli(void);

#endif

