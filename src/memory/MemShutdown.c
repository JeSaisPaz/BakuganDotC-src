// bdc 0x089d7c44 MemShutdown
#include "bdc.h"

/* Destroys the game heap: deletes `g_memMutex` (`MemDeleteMutex`), frees the `MemMng` buffer
   with `free` and clears `g_memMng`. Only called from `MemInit` when a heap already exists. */
void MemShutdown(void)
{
    MemDeleteMutex();
    free(g_memMng);
    g_memMng = NULL;
}
