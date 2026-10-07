// bdc 0x089fd854 IoPacLoaderDelete
#include "bdc.h"

/* Deleting-destructor thunk of the `.pac` package loader: frees `loader` under `MemLock` when bit
   0 of `flags` is set. Byte-identical to `SysUtilCellDelete`; called by `IoPacLoaderDestroy`. */
void IoPacLoaderDelete(void *loader, u32 flags)
{
    if (loader != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(loader, NULL, 0);
        MemUnlock();
    }
}
