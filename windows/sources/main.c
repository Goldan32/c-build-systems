#include <stdio.h>
#include "file.h"

int libFun(int);

int main(int argc, char** argv)
{
    printf("Hello World!\n");
    testFun();
    printf("Static library says 3*3 is %d", libFun(3));

    return 0;
}
