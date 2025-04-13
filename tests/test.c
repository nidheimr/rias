#pragma once

#include <rias.h>
#include <stdlib.h>

void purposely_leaked()
{
    int* bad = malloc(sizeof(int));
}

void properly_handled()
{
    int* good = malloc(sizeof(int));
    free(good);
}