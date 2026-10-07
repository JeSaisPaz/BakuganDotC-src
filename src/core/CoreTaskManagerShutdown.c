// bdc 0x089bfb78 CoreTaskManagerShutdown
#include "bdc.h"

/* Deletes the global task manager: if `g_taskManager` is set, runs `CoreTaskManagerDestroy` on
   it with flags 3 and clears the pointer. Called from the exit path of `BootMainThread`. */
void CoreTaskManagerShutdown(void)
{
    if (g_taskManager != NULL) {
        CoreTaskManagerDestroy(g_taskManager, 3);
        g_taskManager = NULL;
    }
}
