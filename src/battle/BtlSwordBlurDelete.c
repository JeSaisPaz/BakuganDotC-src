// bdc 0x0882a290 BtlSwordBlurDelete
#include "bdc.h"

/* Deleting destructor of the sword-blur trail (`BtlSwordBlurInit`): frees the object when `flags &
   1` (nothing else to release). Called by `BtlBakuganDtor`. */
void BtlSwordBlurDelete(void *blur, u32 flags)
{
    if (blur != NULL && (flags & 1) != 0) {
        MemLock();
        MemFree(blur, NULL, 0);
        MemUnlock();
    }
}
