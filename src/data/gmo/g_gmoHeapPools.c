// bdc 0x08af1340 g_gmoHeapPools
#include "bdc.h"

__typeof__(GmoImagePool[3]) g_gmoHeapPools = {
    { .alloc = (void *(*)(int))malloc, .free = (void (*)(void *))free },
    { .alloc = (void *(*)(int))malloc, .free = (void (*)(void *))free },
    { .alloc = (void *(*)(int))GmoHeapNullAlloc, .free = (void (*)(void *))GmoHeapNullFree },
};
