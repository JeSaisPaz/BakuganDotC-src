// bdc 0x08a321a0 MemLocalHeapContainsPtr
#include "bdc.h"

/* Range-check virtual of the local heap (vtable slot `+0x1c`): always returns false, i.e. no
   pointer is reported as belonging to it. */
bool MemLocalHeapContainsPtr(void *heap, void *ptr)
{
    (void)heap;
    (void)ptr;
    return false;
}
