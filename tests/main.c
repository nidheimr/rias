#define RIAS_IMPLEMENTATION
//#define RIAS_DONT_HIJACK_MAIN
//#define RIAS_DONT_WRAP_STDLIB
//#define RIAS_DONT_DUMP_ON_EXIT
#include <rias.h>
#include <stdlib.h>

#include "test.h"

int main(int argc, char* argv[])
{
    // malloc handled
    int* malloc_handled = malloc(sizeof(int));
    free(malloc_handled);

    // malloc unhandled
    int* malloc_unhandled = malloc(sizeof(int));

    // calloc handled
    int* calloc_handled = calloc(1, sizeof(int));
    free(calloc_handled);

    // calloc unhandled
    int* calloc_unhandled = calloc(1, sizeof(int));

    // realloc handled
    int* realloc_handled = malloc(sizeof(int));
    realloc_handled = realloc(realloc_handled, sizeof(int));
    free(realloc_handled);

    // realloc unhandled
    int* realloc_unhandled = malloc(sizeof(int));
    realloc_unhandled = realloc(realloc_unhandled, sizeof(int));

    // multi file handled
    properly_handled();

    // multi file unhandled
    purposely_leaked();
    
    return 0;
}