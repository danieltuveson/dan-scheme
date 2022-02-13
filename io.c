// Command line interface 
// TODO file loading options?
#include <stdio.h>
#include "sexpression.h"
#include "utils.h"
#include "parser.h"
#include "interpreter.h"
#include "io.h"

void enter_cli(void)
{
    struct Definition **top_level_defs = calloc(MAX_LEVEL_DEFINES, sizeof(struct Definition *));

    printf("Welcome to my Scheme interpreter! Have fun Schemin' 🤙\n");
    while (true)
    {
        printf("> ");

        struct SExpression *sexp;
        sexp = read_sexpression();
        if (sexp == NULL)
        {
            fflush(stdin);
            continue;
        }

        // print contents of sexpression from input
        if (sexp != NULL)
        {
            // print_sexp(sexp, 0);
            struct LispVariable *lvar = eval_top_level(top_level_defs, sexp);
            if (lvar != NULL)
            {
                print_lisp_value(lvar->lv, lvar->lt);
                printf("\n");
            }

            // interpret_sexp(sexp);
        }
    }
}

bool is_word_char(c)
{
    if (strchr("()`'#@ \t\r\n", c))
        return false;
    else
        return true;
}

struct SExpression *read_sexpression(void)
{
    char prevc, c;
    c = fgetc(stdin);
    if (c == '(')
    {
        // Read input until all parens are matched and followed by a newline
        prevc = c;
        c = fgetc(stdin);

        int paren_count;
        paren_count = 0;


        char *new_buff = malloc(sizeof(char) * BUFF_SIZE);
        memset(new_buff, '\0', BUFF_SIZE);
        new_buff[0] = prevc;
        new_buff[1] = c;

        if (prevc == '(' || c == '(') 
            paren_count++;
        else if (prevc == ')' || c == ')')
            paren_count--;

        for (int i = 2; (i < (BUFF_SIZE-1)) && (c != EOF); i++)
        {
            if (c == '(') 
                paren_count++;
            else if (c == ')')
                paren_count--;
            c = fgetc(stdin);
            if ((paren_count == 0) && (c == '\n'))
                break;
            else if ((paren_count < 0) && (c == '\n'))
            {
                printf("Error: unexpected ')'\n");
                break;
            }
            new_buff[i] = c;
            prevc = c;
        }
        if (paren_count < 0)
        {
            printf("Error: unbalanced parentheses\n");
            return NULL;
        }

        int start = 0;
        union LispValue lv;
        lv.sexp = parse_sexpression(new_buff, &start);
        return newSExp(lv, SEXPRESSION);
    }
    else 
    {
        char *buff = malloc(sizeof(char) * BUFF_SIZE);
        int i;
        for (i = 0; c != '\n'; i++, c = fgetc(stdin))
        {
            if (i == BUFF_SIZE)
            {
                printf("Exceeded maximum size of %d characters for a word\n", BUFF_SIZE);
                return NULL;
            }
            else if (!is_word_char(c))
            {
                printf("Error: '%c' not expected at this time\n", c);
                return NULL;
            }
            else 
            {
                buff[i] = c;
            }
        }
        struct SExpression *sexp = parse_word(NULL, buff);
        free(buff);
        return sexp;
    }
}

