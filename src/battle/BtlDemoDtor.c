// bdc 0x088ff5ac BtlDemoDtor
#include "bdc.h"

/* Destructor of the battle intro demo task `BtlDemo` (task id 0x65, vtable `g_btlDemoVtbl`,
   `BtlDemoCtor`; also chained to by `BtlAppearDemoDtor`): does nothing for NULL; reinstalls
   its vtable, clears the actor list (`ActorClearList`), destroys the embedded demo camera
   (`BtlDemoCamDtor`) and the three embedded packages last-first (`IoLzsPackageDtor`, flags 2:
   no free), runs the base `CoreTaskDestroy` and frees the task (under `MemLock`) when `flags &
   1`. */
void BtlDemoDtor(CoreTask *task, u32 flags)
{
    BtlDemo *demo = (BtlDemo *)task; /* task is the CoreTask base at +0 */

    if (demo != NULL) {
        demo->base.vtable = g_btlDemoVtbl;
        ActorClearList();
        BtlDemoCamDtor(&demo->cam, 2);
        IoLzsPackageDtor(&demo->packages[2], 2);
        IoLzsPackageDtor(&demo->packages[1], 2);
        IoLzsPackageDtor(&demo->packages[0], 2);
        CoreTaskDestroy(&demo->base, 0);
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(demo, NULL, 0);
            MemUnlock();
        }
    }
}
