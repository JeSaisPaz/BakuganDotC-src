// bdc 0x08af57ac g_uiSpriteMngVtable
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiSpriteMngVtable = {
    {0}, { .fn = (void *)UiSpriteMngTaskDtor }, { .fn = (void *)UiSpriteMngTaskUpdateNop },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiSpriteMngTaskDraw },
    { .fn = (void *)UiSpriteMngTaskSetField }, { .fn = (void *)UiSpriteMngTaskGetField },
};
