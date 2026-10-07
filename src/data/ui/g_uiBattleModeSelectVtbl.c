// bdc 0x08af516c g_uiBattleModeSelectVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiBattleModeSelectVtbl = {
    {0}, { .fn = (void *)UiBattleModeSelectDtor }, { .fn = (void *)UiBattleModeSelectUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiBattleModeSelectDraw },
    { .fn = (void *)UiBattleModeSelectSetField }, { .fn = (void *)UiBattleModeSelectGetField },
};
