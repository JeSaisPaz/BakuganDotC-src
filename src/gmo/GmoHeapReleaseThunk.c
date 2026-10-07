// bdc 0x08a133e8 GmoHeapReleaseThunk
#include "bdc.h"

/* Thunk (`j` + `nop`) to `GmoHeapRelease`: drops a reference to the model-library heap block
   holding `ptr`, starting at pool `pool`; a block reaching 0 is freed. Returns `ptr`. Most of the
   GMO model/mesh/motion release code calls the heap through this thunk. */
void *GmoHeapReleaseThunk(int pool, void *ptr)
{
    /* `j 0x08a13214`: Ghidra inlined the target's body; the binary only tail-jumps to it. */
    return GmoHeapRelease(pool, ptr);
}
