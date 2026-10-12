// bdc 0x08af4d44 g_uiBattleRecordVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiBattleRecordVtbl = {
    {0}, { .fn = (void *)UiBattleRecordDtor }, { .fn = (void *)UiBattleRecordUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiBattleRecordDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
