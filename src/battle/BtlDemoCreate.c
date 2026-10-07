// bdc 0x088fecc8 BtlDemoCreate
#include "bdc.h"

/* Creates the battle demo task (task id 0x65) for demo `demoId`: allocates 0x790 bytes from the
   low end of the heap, constructs it, sets task id 0x65 and task flag 2, inserts it at priority
   100 and, when the battle main task (id 100) exists, runs one scene update on it. A failed
   allocation is not handled: the id is then written through NULL, as in the original. Returns
   the task. */
BtlDemo *BtlDemoCreate(u32 demoId)
{
    bool fromLow;
    BtlDemo *mem;
    BtlDemo *demo;
    BtlMain *battle;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(0x790, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    demo = NULL;
    if (mem != NULL) {
        BtlDemoCtor(&mem->base, demoId);
        demo = mem;
    }
    demo->base.id = 0x65;
    CoreTaskSetFlags(&demo->base, 2);
    CoreTaskInsert(demo, 100);
    battle = CoreTaskFind(100);
    if (battle != NULL) {
        BtlMainUpdateScene(battle);
    }
    return demo;
}
