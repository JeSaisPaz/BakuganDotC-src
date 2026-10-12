// bdc 0x08af4884 g_uiConfirmDialogVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiConfirmDialogVtbl = {
    {0}, { .fn = (void *)UiConfirmDialogDtor }, { .fn = (void *)UiConfirmDialogUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiConfirmDialogDraw },
    { .fn = (void *)UiConfirmDialogSetField }, { .fn = (void *)UiConfirmDialogGetField },
};
