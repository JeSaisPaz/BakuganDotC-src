// bdc 0x089db100 GmoDlCtxInit
#include "bdc.h"

/* Initialises the head of a GMO display-list context: write pointer and start = `*buf` (or 0), end
   = start + `*size & ~3` (or -4 without a size), and the caller's flags word at `+0xc`. */

void GmoDlCtxInit(GmoDlContext *self, void **buf, u32 *size, u32 flags)
{
    u32 *start = NULL;
    u32 *end;

    if (buf != NULL) {
        start = *buf;
    }
    if (start == NULL) {
        self->cur = NULL;
        end = NULL;
    } else {
        end = (u32 *)~(uintptr_t)3;
        if (size != NULL) {
            end = (u32 *)((u8 *)start + (*size & ~3u));
        }
        self->cur = start;
    }
    self->start = start;
    self->end = end;
    self->flags = flags;
}
