// bdc 0x0889d934 BtlStageDestroyEventScript
#include "bdc.h"

/* Destroys the battle event script `g_btlEventScript` (started by `BtlStageLoadMap` with
   `ScriptSpawn`) through its virtual deleting destructor (vtable entry 1, flag 3) and clears the
   pointer. Called by `BtlMainTaskDtor` and `BtlMainTeardown`. */
void BtlStageDestroyEventScript(void)
{
    if (g_btlEventScript != NULL) {
        const VtblEntry *dtor = &((const VtblEntry *)g_btlEventScript->vtable)[1];
        ((void (*)(void *, s32))dtor->fn)((u8 *)g_btlEventScript + dtor->delta, 3);
        g_btlEventScript = NULL;
    }
}
