// bdc 0x08af2d84 g_gameDebugStageSelectVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_gameDebugStageSelectVtbl = {
    {0}, { .fn = (void *)GameDebugStageSelectDtor }, { .fn = (void *)GameDebugStageSelectUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)GameDebugStageSelectDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
