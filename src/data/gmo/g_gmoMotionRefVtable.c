// bdc 0x08af5424 g_gmoMotionRefVtable
#include "bdc.h"

__typeof__(VtblEntry[12]) g_gmoMotionRefVtable = {
    {0}, { .fn = (void *)GmoMotionRefDtor }, { .fn = (void *)GmoMotionRefGetInfo },
    { .fn = (void *)GmoMotionBaseSetName }, { .fn = (void *)GmoMotionRefGetName },
    { .fn = (void *)GmoMotionRefNameEquals }, { .fn = (void *)GmoMotionBaseReserveArena },
    { .fn = (void *)GmoMotionBaseArenaAlloc }, { .fn = (void *)GmoMotionRefGetArena },
    { .fn = (void *)GmoMotionRefGetArenaSize }, { .fn = (void *)GmoMotionBaseSetArena },
    { .fn = (void *)GmoMotionRefInitInPlace },
};
