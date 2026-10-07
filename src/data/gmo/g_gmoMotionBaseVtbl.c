// bdc 0x08af6fe8 g_gmoMotionBaseVtbl
#include "bdc.h"

__typeof__(VtblEntry[12]) g_gmoMotionBaseVtbl = {
    {0}, { .fn = (void *)GmoMotionBaseDtor }, { .fn = (void *)GmoMotionBaseGetInfo },
    { .fn = (void *)GmoMotionBaseSetName }, { .fn = (void *)GmoMotionBaseGetName },
    { .fn = (void *)GmoMotionBaseNameEquals }, { .fn = (void *)GmoMotionBaseReserveArena },
    { .fn = (void *)GmoMotionBaseArenaAlloc }, { .fn = (void *)GmoMotionBaseGetArena },
    { .fn = (void *)GmoMotionBaseGetArenaSize }, { .fn = (void *)GmoMotionBaseSetArena },
    { .fn = (void *)GmoMotionBaseInitFromBin },
};
