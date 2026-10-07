// bdc 0x08af15ac g_saveLanguageTaskVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_saveLanguageTaskVtbl = {
    {0}, { .fn = (void *)SaveLanguageTaskDtor }, { .fn = (void *)SaveLanguageTaskUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)CoreTaskBaseDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
