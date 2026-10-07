// bdc 0x08af17fc g_btlArenaPhotoTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_btlArenaPhotoTaskVtbl = {
    {0}, { .fn = (void *)BtlArenaPhotoTaskDtor }, { .fn = (void *)BtlArenaPhotoTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
