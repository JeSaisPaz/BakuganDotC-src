// bdc 0x08af1258 g_gmoImagePools
#include "bdc.h"

__typeof__(GmoImagePool[3]) g_gmoImagePools = {
    { .alloc = (void *(*)(int))malloc, .free = (void (*)(void *))free },
    { .alloc = (void *(*)(int))malloc, .free = (void (*)(void *))free },
    {
        .alloc = (void *(*)(int))GmoImageHeapNullAlloc,
        .free = (void (*)(void *))GmoImageHeapNullFree,
    },
};
