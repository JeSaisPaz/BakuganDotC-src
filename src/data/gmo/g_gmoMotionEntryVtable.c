// bdc 0x08af53c4 g_gmoMotionEntryVtable
#include "bdc.h"

__typeof__(VtblEntry[12]) g_gmoMotionEntryVtable = {
    {0}, { .fn = (void *)GmoMotionEntryDtor }, { .fn = (void *)GmoMotionEntryGetInfo },
    { .fn = (void *)GmoMotionEntrySetName }, { .fn = (void *)GmoMotionEntryGetName },
    { .fn = (void *)GmoMotionEntryNameEquals }, { .fn = (void *)GmoMotionEntryReserveArena },
    { .fn = (void *)GmoMotionEntryArenaAlloc }, { .fn = (void *)GmoMotionEntryGetArena },
    { .fn = (void *)GmoMotionEntryGetArenaSize }, { .fn = (void *)GmoMotionEntrySetArena },
    { .fn = (void *)GmoMotionEntryInitFromBin },
};
