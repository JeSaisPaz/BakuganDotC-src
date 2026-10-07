// bdc 0x08a0375c CxxEhRunCleanups
#include "bdc.h"

/* Runs the cleanup actions of a cleanup-region frame (`CxxEhCleanupFrame`) from entry `from` up
   to (not including) entry `to`, following each entry's `next` link: calls destructors
   (`dtor(obj, 2)`, or 0 for flag 0x40), array destructors (`CxxVecDelete`) or deallocators
   (non-NULL objects only, sized with flag 8), or clears a pointer (flag 0x80); entries guarded by
   flag 2 are skipped when their guard variable is 0. */

void CxxEhRunCleanups(void *frame, u32 from, u32 to)
{
    CxxEhCleanupFrame *cf = (CxxEhCleanupFrame *)frame;
    void **objects = cf->objects;
    u32 idx = from & 0xffff;
    u32 end = to & 0xffff;

    while (idx != end) {
        CxxEhCleanupEntry *ent = &cf->entries[idx];
        u8 fl = ent->flags;
        CxxEhArrayInfo *info = NULL;
        void *ptr;

        if ((fl & 2) != 0 && *(int *)objects[cf->entries[idx + 1].object] == 0) {
            idx = ent->next;
            continue;
        }
        if ((fl & 8) != 0) {
            info = &cf->arrays[ent->object];
            ptr = objects[info->object];
        } else {
            ptr = objects[ent->object];
        }
        if ((fl & 1) != 0) {
            ptr = *(void **)ptr;
        }
        if ((fl & 0x80) != 0) {
            *(void **)ptr = NULL;
        } else if ((fl & 4) == 0) {
            if ((fl & 8) != 0) {
                CxxVecDelete(ptr, info->count, info->elemSize, ent->fn, 0, 0);
            } else {
                ((void (*)(void *, int))ent->fn)(ptr, (fl & 0x40) != 0 ? 0 : 2);
            }
        } else if (ptr != NULL) {
            if ((fl & 8) != 0) {
                ((void (*)(void *, u32))ent->fn)(ptr, info->elemSize);
            } else {
                ((void (*)(void *))ent->fn)(ptr);
            }
        }
        idx = ent->next;
    }
}
