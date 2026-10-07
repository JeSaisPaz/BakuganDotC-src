// bdc 0x089056f8 BtlDemoScenePlayerCreate
#include "bdc.h"

/* Allocates (0x70 bytes from the low heap, under `MemLock`) and constructs a battle demo scene
   player task `BtlDemoScenePlayer` with `BtlDemoScenePlayerCtor``(task, demoId, demo, actor,
   follower)`, then sets its task id to 0x66 and inserts it at priority 100 (`CoreTaskInsert`).
   Returns the task. When the allocation fails the binary still stores the id through the NULL
   pointer and inserts NULL. */
BtlDemoScenePlayer *BtlDemoScenePlayerCreate(s32 demoId, void *demo, Actor *actor, void *follower)
{
    bool fromLow;
    BtlDemoScenePlayer *mem;
    BtlDemoScenePlayer *task;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0x70, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    task = NULL;
    if (mem != NULL) {
        BtlDemoScenePlayerCtor(mem, demoId, demo, actor, follower);
        task = mem;
    }
    /* No NULL check in the binary: a failed allocation writes the id at address 0. */
    task->base.id = 0x66;
    CoreTaskInsert(&task->base, 100);
    return task;
}
