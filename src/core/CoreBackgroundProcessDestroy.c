// bdc 0x089ff7d0 CoreBackgroundProcessDestroy
#include "bdc.h"

/* Destroys the `COBackGroundProcess` worker singleton `g_coreBackgroundProcess`
   (`CoreBackgroundProcessDtor``(proc, 3)`) and clears it. Called by
   `BootBackgroundProcessThread` when its job loop ends. */
void CoreBackgroundProcessDestroy(void)
{
    if (g_coreBackgroundProcess != NULL) {
        CoreBackgroundProcessDtor(g_coreBackgroundProcess, 3);
        g_coreBackgroundProcess = NULL;
    }
}
