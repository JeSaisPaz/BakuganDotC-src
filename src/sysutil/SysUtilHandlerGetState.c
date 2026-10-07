// bdc 0x089cbda8 SysUtilHandlerGetState
#include "bdc.h"

/* Returns a system-utility handler's dialog state word: 0 none, 1 initialising, 2 visible,
   3 quit requested, 4 finished (the PSP_UTILITY_DIALOG_* values). */
s32 SysUtilHandlerGetState(SysUtilHandler *self)
{
    return self->state;
}
