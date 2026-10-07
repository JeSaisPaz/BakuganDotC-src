// bdc 0x089b4ad4 _reclaim_reent
#include "bdc.h"

/* Newlib's `_reclaim_reent(ptr)`: releases everything a thread's `_reent` owns. It does nothing
   for the global instance (`ptr == ``g_impurePtr`). Otherwise it frees the 15 bucket chains of
   the bigint `_freelist` and the array itself, the extra `_atexit` blocks up to (excluding) the
   embedded `_atexit0`, the `_cvtbuf`, and, if stdio was initialised (`__sdidinit`), calls
   `__cleanup(ptr)` and frees the `_glue` chain with `cleanup_glue`. */

void _reclaim_reent(_reent *ptr)
{
    s32 i;
    void **node;
    void **next;
    _atexit *block;
    _atexit *nextBlock;

    if (ptr == g_impurePtr) {
        return;
    }
    if (ptr->_freelist != NULL) {
        for (i = 0; i < 15; i++) {
            node = ptr->_freelist[i];
            while (node != NULL) {
                next = *node;
                _free_r(ptr, node);
                node = next;
            }
        }
        _free_r(ptr, ptr->_freelist);
    }
    block = ptr->_atexit;
    if (block != NULL) {
        while (block != &ptr->_atexit0) {
            nextBlock = block->_next;
            _free_r(ptr, block);
            block = nextBlock;
        }
    }
    if (ptr->_cvtbuf != NULL) {
        _free_r(ptr, ptr->_cvtbuf);
    }
    if (ptr->__sdidinit != 0) {
        ((void (*)(_reent *))ptr->__cleanup)(ptr);
        if (ptr->__sglue._next != NULL) {
            cleanup_glue(ptr, ptr->__sglue._next);
        }
    }
}
