#include <assert.h>
#include <stdio.h>
#include "utils.h"
#include "sexpression.h"

void test_stuff()
{
    println("... runnning tests ...");
    union LispValue c0, c1, c2;
    struct SExpression *s0, *s1;

    // testing new
    println("... testing new ...");
    c0.atom = calloc(BUFF_SIZE, sizeof(char));
    strcpy(c0.atom, "this-is-an-atom");
    s0 = newSExp(c0, ATOM);
    assert(strcmp(s0->car.atom, "this-is-an-atom") == 0);
    assert(s0->cdr == NULL);

    // testing concat
    println("... testing concat ...");
    c1.atom = calloc(BUFF_SIZE, sizeof(char));
    strcpy(c1.atom, "this-is-another-string");
    s1 = newSExp(c1, ATOM);
    concat(s1, s0);
    assert(strcmp(s1->cdr->car.atom, "this-is-an-atom") == 0);
    assert(strcmp(s1->car.atom, "this-is-another-string") == 0);
    assert(s1->cdr->cdr == NULL);

    // testing change to sexpression element
    strcpy(s0->car.atom, "s0-is-changed");
    assert(strcmp(s1->cdr->car.atom, "s0-is-changed") == 0);

    // testing append
    println("... testing append ...");
    c2.atom = calloc(BUFF_SIZE, sizeof(char));
    strcpy(c2.atom, "another-atom");
    append(s1, c2, ATOM);
    assert(strcmp(s1->car.atom, "this-is-another-string") == 0);
    assert(strcmp(s1->cdr->car.atom, "s0-is-changed") == 0);
    assert(strcmp(s1->cdr->cdr->car.atom, "another-atom") == 0);
    assert(s1->cdr->cdr->cdr == NULL);

    println("... tests finished ...");
}
