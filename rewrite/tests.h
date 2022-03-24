#ifndef TESTS
#define TESTS

#define RUN_TEST(name) printf("... testing %s ...\n", #name); test_ ## name()

#include "tests.c.generated.h"
#endif
