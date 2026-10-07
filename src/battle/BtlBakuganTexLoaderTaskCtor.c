// bdc 0x088700c0 BtlBakuganTexLoaderTaskCtor
#include "bdc.h"

/* Constructor of the Bakugan texture loader task (`BtlBakuganTexLoaderTask`, task id 483 =
   0x1e3, 0x40 bytes): runs `CoreTaskInit`, installs `g_btlBakuganTexLoaderTaskVtbl`, sets
   `step` to 0 and the four `kinds` slots to -1, resets `g_btlTexLoaderCount` and allocates
   (from the low end of the heap) and constructs a 0x44-byte `IoLzsPackageCtor` package in
   `package` (NULL when the allocation fails). Returns `task`. */

CoreTask *BtlBakuganTexLoaderTaskCtor(CoreTask *task)
{
    BtlBakuganTexLoaderTask *self = (BtlBakuganTexLoaderTask *)task;
    bool fromLow;
    IoLzsPackage *mem;
    IoLzsPackage *package;
    int i;

    CoreTaskInit(task);
    self->base.vtable = g_btlBakuganTexLoaderTaskVtbl;
    self->step = 0;
    for (i = 0; i < 4; i++) {
        self->kinds[i] = -1;
    }
    g_btlTexLoaderCount = 0;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = (IoLzsPackage *)MemAlloc(0x44, NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    package = NULL;
    if (mem != NULL) {
        IoLzsPackageCtor(mem);
        package = mem;
    }
    self->package = package;
    return task;
}
