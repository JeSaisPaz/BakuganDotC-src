// bdc 0x08a10820 GmoImageHeapReleaseThunk
#include "bdc.h"

/* Thunk (`j` + `nop`) to `GmoImageHeapRelease`: drops a reference to the image-library heap block
   holding `ptr`, starting at pool `pool`. Returns `ptr`. The texture/image release and build code
   calls the image heap through this thunk. */
void *GmoImageHeapReleaseThunk(int pool, void *ptr)
{
    /* `j 0x08a1075c`: Ghidra inlined the target's body; the binary only tail-jumps to it. */
    return GmoImageHeapRelease(pool, ptr);
}
