// bdc 0x089bac84 _sbrk_r
#include "bdc.h"

/* Newlib's reentrant syscall wrapper `_sbrk_r(ptr, incr)`: clears the global `g_errno`, calls
   `_sbrk`(incr), and if that returned -1 while `errno` became non-zero, copies it into
   `ptr->_errno`. Returns the `_sbrk` result. */
void *_sbrk_r(_reent *ptr, s32 incr)
{
    void *ret;

    g_errno = 0;
    ret = _sbrk(incr);
    if (ret == (void *)(intptr_t)-1 && g_errno != 0) {
        ptr->_errno = g_errno;
    }
    return ret;
}
