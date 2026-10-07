// bdc 0x089ff808 CoreGetBackgroundProcess
#include "bdc.h"

/* Returns `g_coreBackgroundProcess` (singleton accessor, named by `bdc singleton`). */
void *CoreGetBackgroundProcess(void)
{
    return g_coreBackgroundProcess;
}
