// bdc 0x08af4964 g_uiPauseVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiPauseVtbl = {
    {0}, { .fn = (void *)UiPauseDtor }, { .fn = (void *)UiPauseUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiPauseDraw },
    { .fn = (void *)UiPauseSetField }, { .fn = (void *)UiPauseGetField },
};
