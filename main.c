#include "io.h"
#include "tests.h"

#define RUN_TESTS 1
int main() 
{
#if RUN_TESTS
    tests();
#endif 

    mainLoop();
    return 0;
}
