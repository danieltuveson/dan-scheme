#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "parser.h"
#include "utils.h"
#include "interpreter.h"

// TODO deal with leading whitespace before first paren
struct SExpression *parse_sexpression(char buffer[BUFF_SIZE], int *start_index)
{
    char current_word[BUFF_SIZE];
    int i, c, prevc, start_token;
    c = buffer[*start_index];
    if (c != '(') 
    {
        println("Parse error: sexpression must begin with '('");
        return NULL;
    }

    union LispValue lv;
    struct SExpression *sexp = NULL;
    prevc = c;
    start_token = *start_index + 1;
    memset(current_word, '\0', BUFF_SIZE);

    for (i = *start_index + 1; i < BUFF_SIZE; i++)
    {
        c = buffer[i];
        // printf("start index: %d, ", *start_index);
        // printf("i: %d, ", i);
        // printf("c: '%c', ", c);
        // printf("prevc: '%c'", prevc);
        // printf("current word: '%s'\n", current_word);
        if (c == '(')
        {
//             // first element of list must be an atom
//             if (i == *start_index + 1) 
//             {
//                 println("Parse error: first element of s-expression must be an atom");
//                 return NULL;
//             }
            int old_start = *start_index;
            *start_index = i;
            struct SExpression *sub_sexp = parse_sexpression(buffer, start_index);
            i = *start_index;
            *start_index = old_start;
            lv.sexp = sub_sexp;
            sexp = append(sexp, lv, SEXPRESSION);
            // print_sexp(sexp, 0);
            // treat s-expression like a space character
            c = ' ';
        }
        if (c == '\0' || (c == ')'))
        {
            break;
        }
        else if ((!isspace(prevc)) && isspace(c))
        {
            if (current_word[0] != '\0') 
            {
                sexp = parse_word(sexp, current_word);
                memset(current_word, '\0', BUFF_SIZE);
            }
        }
        else if (isspace(prevc) && (!isspace(c)))
        {
            start_token = i;
            current_word[i-start_token] = buffer[i];
        }
        else if ((!isspace(prevc)) && (!isspace(c)))
        {
            current_word[i-start_token] = buffer[i];
        }
        prevc = c;
    }
    if (c != ')') 
    {
        println("Parse error: sexpression must end with ')'");
        delete_sexpression(sexp);
        return NULL;
    }
    if (current_word[0] != '\0') 
    {
        sexp = parse_word(sexp, current_word);
    }
    *start_index = i;
    return sexp;
}

// checks if number is actually zero when zero is returned from atoi
int is_zero(char *str)
{
    if (str[0] != '0')
        return false;
    for (int i = 1; str[i] != '\0'; i++)
    {
        if (str[i] == '\0')
            return true;
        else if (str[i] != '0')
            continue;
        else
            return false;
    }
    return false;
}

// Parses word and returns word appened to end of sexp 
struct SExpression *parse_word(struct SExpression *sexp, char *word)
{
    union LispValue lv;
    int num = atoi(word);
    bool is_number = (num != 0) || is_zero(word);

    // if (word[0] == '"')
    // {
    //     // printf("string!\n");
    // }
    // else 
    if (is_number)
    {
        lv.num = num;
        sexp = append(sexp, lv, NUM);
    }
    else
    {
        lv.atom = malloc(sizeof(char) * BUFF_SIZE);
        strcpy(lv.atom, word);
        sexp = append(sexp, lv, ATOM);
        free(lv.atom);
        // printf("atom!!\n");
    }

    // printf("word: %s\n", word);

    return sexp;
}

