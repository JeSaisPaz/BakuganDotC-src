// bdc 0x08af4d0c g_uiStaffCreditVtbl
#include "bdc.h"

__typeof__(VtblEntry[7]) g_uiStaffCreditVtbl = {
    {0}, { .fn = (void *)UiStaffCreditDtor }, { .fn = (void *)UiStaffCreditUpdate },
    { .fn = (void *)CoreTaskBaseSlot3Nop }, { .fn = (void *)UiStaffCreditDraw },
    { .fn = (void *)CoreTaskSetField }, { .fn = (void *)CoreTaskGetField },
};
