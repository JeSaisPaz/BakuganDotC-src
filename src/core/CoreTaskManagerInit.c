// bdc 0x089bf358 CoreTaskManagerInit
#include "bdc.h"

/* Constructor body of the task manager (`this` is the 4-byte object made by
   `CoreTaskManagerCreate`): allocates the 0x10-byte list header from the low heap and builds
   `g_taskList` with `CoreListInit` (32-node pool; NULL on allocation failure), then
   initialises the shared null texture (`GfxInitNullTexture`, see `g_nullTexture`) and the BGM
   command list (`SndBgmCmdListCreate` with 8 entries). Returns `this`. */
void *CoreTaskManagerInit(void *this)
{
    bool fromLow;
    CoreList *list;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    list = MemAlloc(0x10, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (list != NULL)
        CoreListInit(list, 0x20);
    g_taskList = list;
    GfxInitNullTexture();
    SndBgmCmdListCreate(8);
    return this;
}
