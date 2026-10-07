// bdc 0x08a12674 GmoImageArrayReleaseThunk
#include "bdc.h"

/* Thunk (`j` + `nop`) to `GmoImageArrayRelease`: drops one reference on each of `n` consecutive
   0x30-byte image/palette records, destroying and freeing those that reach 0. Returns `arr`. Used
   by `GmoTextureDestroyContents` and `GmoTextureReleaseWritable`. */
short *GmoImageArrayReleaseThunk(short *arr, int n)
{
    /* `j 0x08a125e4`: Ghidra inlined the target's body; the binary only tail-jumps to it. */
    return GmoImageArrayRelease(arr, n);
}
