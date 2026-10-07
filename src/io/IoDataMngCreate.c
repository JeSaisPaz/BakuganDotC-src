// bdc 0x089fcd7c IoDataMngCreate
#include "bdc.h"

/* Creates the data manager (`CODataMng`, `g_ioDataMng`) (0x14 bytes, `IoDataMngCtor`) and the
   owner-reference pool (`IoDataRefPoolCreate`); returns it. Called by `BootDataThread`. */

void *IoDataMngCreate(void)
{
    bool fromLow;
    IoDataMng *self;
    IoDataMng *mng;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    self = MemAlloc(sizeof(IoDataMng), (char *)0x0, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    mng = (IoDataMng *)0x0;
    if (self != (IoDataMng *)0x0) {
        IoDataMngCtor(self);
        mng = self;
    }
    g_ioDataMng = mng;
    IoDataRefPoolCreate();
    return g_ioDataMng;
}
