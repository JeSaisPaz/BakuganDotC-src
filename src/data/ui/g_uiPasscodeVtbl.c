// bdc 0x08af4b5c g_uiPasscodeVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiPasscodeVtbl = {
    {0}, { .fn = (void *)UiPasscodeDtor }, { .fn = (void *)UiPasscodeUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiPasscodeDraw },
    { .fn = (void *)UiPasscodeSetField }, { .fn = (void *)UiPasscodeGetField },
};
