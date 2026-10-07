// bdc 0x08af2d4c g_uiTalkBalloonVtbl
#include "bdc.h"

__typeof__(VtblEntry[5]) g_uiTalkBalloonVtbl = {
    {0}, { .fn = (void *)UiTalkBalloonDtor }, { .fn = (void *)UiTalkBalloonUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiTalkBalloonDraw },
};
