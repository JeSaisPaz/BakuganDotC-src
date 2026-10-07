// bdc 0x08884874 BtlInputDtor
#include "bdc.h"

/* Deleting destructor of the input controller (`g_btlInputVtbl` entry 1): re-installs the
   vtable and frees the object when `flags & 1`. Does nothing for NULL. */
void BtlInputDtor(BtlInput *self, u32 flags)
{
    if (self != NULL) {
        self->vtbl = g_btlInputVtbl;
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(self, NULL, 0);
            MemUnlock();
        }
    }
}
