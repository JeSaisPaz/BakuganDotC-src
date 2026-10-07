// bdc 0x089bc334 BootDiscThread
#include "bdc.h"

/* Entry of thread slot 2 "MyThread-Disc": `IoUmdInit`, `IoDiscCreate`, then calls the disc
   manager's virtual update (vtable at `+0x100`, slot `+0xc`) while `IoDiscHasManager`, and
   finally `IoDiscDestroy` and `IoUmdShutdown`. Returns 0. */

int BootDiscThread(void)
{
    IoUmdInit();
    IoDiscCreate();
    while (IoDiscHasManager()) {
        IoDiscSimple *mgr = (IoDiscSimple *)IoDiscGetManager();
        const struct VtblEntry *e = &mgr->vtbl[1];

        ((void (*)(void *))e->fn)((char *)mgr + e->delta);
    }
    IoDiscDestroy();
    IoUmdShutdown();
    return 0;
}
