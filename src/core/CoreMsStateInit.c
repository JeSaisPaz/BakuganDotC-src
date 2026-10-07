// bdc 0x089fb0b0 CoreMsStateInit
#include "bdc.h"

/* Constructor of `CoreMs`: creates the kernel callback "MyCB-MS" (`sceKernelCreateCallback`
   with `CoreMsCallback`) and, when that works, stores its id and registers it for Memory Stick
   insert/eject events with `sceIoDevctl("fatms0:", 0x02415821, &id, 4, NULL, 0)`. Clears the
   request flags and the path, sets `writeProtect = -1`, `writeProtected = 1` and zero-fills
   `unk119`. Returns `ms`. */
CoreMs *CoreMsStateInit(CoreMs *ms)
{
    int cbId;

    cbId = sceKernelCreateCallback("MyCB-MS", CoreMsCallback, NULL);
    if (cbId >= 0) {
        ms->callbackId = cbId;
        sceIoDevctl("fatms0:", 0x2415821, &cbId, 4, NULL, 0);
    }
    ms->statPending = 0;
    ms->pathExists = 0;
    ms->path[0] = '\0';
    ms->mkdirPending = 0;
    ms->mkdirDone = 0;
    ms->resetRequest = 0;
    ms->unk109 = 0;
    ms->unk10a = 0;
    ms->writeProtect = -1;
    ms->writeProtected = 1;
    memset(ms->unk119, 0, 0x10);
    return ms;
}
