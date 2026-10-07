// bdc 0x089b4980 _write
#include "bdc.h"

/* Newlib's `_write(fd, buf, len)` system-call stub. File descriptors 0, 1 and 2 are first mapped to
   the kernel's stdio handles (`sceKernelStdin()`, `sceKernelStdout()`, `sceKernelStderr()`); any
   other descriptor is passed through as a PSP file UID. A negative resulting handle gives -9
   (`EBADF`); otherwise returns `sceIoWrite(handle, buf, len)`. */

int _write(int fd, void *buf, u32 len)
{
    if (fd == 0) {
        fd = sceKernelStdin();
    } else if (fd == 1) {
        fd = sceKernelStdout();
    } else if (fd == 2) {
        fd = sceKernelStderr();
    }
    if (fd < 0) {
        return -9;
    }
    return sceIoWrite(fd, buf, len);
}
