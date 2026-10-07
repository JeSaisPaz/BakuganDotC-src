// bdc 0x089bf1f8 CoreTaskInit
#include "bdc.h"

/* Base constructor of every `CoreTask`: installs the base vtable `g_coreTaskVtbl`, sets
   `id = -1`, `flags = 0`, clears `child` and returns `task`. Called first by the constructors of
   all task classes (e.g. `SndBgmCmdInit`, `UiLoadIconInit`). */
CoreTask *CoreTaskInit(CoreTask *task)
{
    task->vtable = g_coreTaskVtbl;
    task->id = -1;
    task->flags = 0;
    task->child = NULL;
    return task;
}
