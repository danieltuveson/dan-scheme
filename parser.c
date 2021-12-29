#include "parser.h"

// TODO deal with leading whitespace before first element
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

    struct SExpression *sexp = NULL;
    union LispValue lv;
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
        if (c == '\0' || c == ')') 
        {
            break;
        }
        else if (prevc != ' ' && c == ' ')
        {
            if (current_word[0] != '\0') 
            {
                lv.atom = malloc(sizeof(char) * BUFF_SIZE);
                strcpy(lv.atom, current_word);
                sexp = append(sexp, lv, ATOM);
                free(lv.atom);
                memset(current_word, '\0', BUFF_SIZE);
            }
        }
        else if (prevc == ' ' && c != ' ')
        {
            start_token = i;
            current_word[i-start_token] = buffer[i];
        }
        else if (prevc != ' ' && c != ' ')
        {
            current_word[i-start_token] = buffer[i];
        }
        prevc = c;
        // print_sexp(sexp, 0);
    }
    if (c != ')') 
    {
        println("Parse error: sexpression must end with ')'");
        delete_sexpression(sexp);
        return NULL;
    }
    if (current_word[0] != '\0') 
    {
        lv.atom = malloc(sizeof(char) * BUFF_SIZE);
        strcpy(lv.atom, current_word);
        sexp = append(sexp, lv, ATOM);
        free(lv.atom);
    }
    *start_index = i;
    return sexp;
}

void enter_cli(void)
{
    while (true)
    {
        printf("> ");
        // Read input
        char new_buff[BUFF_SIZE];
        //char test_command[] = "(thing (and another) thing)";
        memset(new_buff, '\0', BUFF_SIZE);
        //strcpy(new_buff, test_command);
        // println(new_buff);
        fgets(new_buff, BUFF_SIZE, stdin);

        struct SExpression *sexp, *head;
        int start = 0;
        sexp = parse_sexpression(new_buff, &start);
        head = sexp;

        // print contents of sexpression from input
        print_sexp(sexp, 0);
    }
}

