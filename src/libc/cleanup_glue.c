// bdc 0x089b4a88 cleanup_glue
#include "bdc.h"

/* Frees a chain of stdio `_glue` nodes: recurses to the end of the `_next` list first and then
   `_free_r`s each node on the way back, so the whole chain is released from the tail. Helper of
   `_reclaim_reent`. */

void cleanup_glue(_reent *ptr, _glue *glue)
{
    if (glue->_next != NULL) {
        cleanup_glue(ptr, glue->_next);
    }
    _free_r(ptr, glue);
}
