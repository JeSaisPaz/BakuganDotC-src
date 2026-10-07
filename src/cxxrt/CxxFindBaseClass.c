// bdc 0x08a0458c CxxFindBaseClass
#include "bdc.h"

/* Recursive base-class search used for catch matching and dynamic casts. `obj` points to the object
   address (or is NULL), `derived` and `base` are type descriptors. First scans the direct
   bases of `derived` for `base` (same descriptor or equal non-null name): an ambiguous match (flag 8)
   stops the search and returns 0; an accessible match (flag 4, or the next `'Y'` character of
   `*access` when `useAccess`) returns 1 and stores the subobject address in `*out` (through the
   virtual-base pointer for flag 1). Otherwise recurses into each base that has its own base table
   (and is accessible and unambiguous unless `useAccess`), returning 1 with that result's `*out` on
   the first hit; returns 0 when nothing matches. `*out` is cleared on entry and only written when
   `obj` gives an object address. */

int CxxFindBaseClass(void *obj, void *out, const CxxTypeInfo *derived, const CxxTypeInfo *base, char **access, int useAccess)
{
    void **outPtr = (void **)out;
    const CxxBaseClassEntry *entry = derived->bases;
    int found = 0;
    int ambiguous = 0;
    u8 *objBase = NULL;
    u8 flags;
    int accessible;

    if (obj != NULL) {
        objBase = *(u8 **)obj;
    }
    *outPtr = NULL;
    if (entry == NULL) {
        return found;
    }

    /* direct bases */
    do {
        const CxxTypeInfo *type = entry->type;
        u8 *sub = NULL;

        if (objBase != NULL) {
            sub = objBase + entry->offset;
        }
        if (useAccess != 0) {
            char *acc = *access;
            if (acc != NULL) {
                *access = acc + 1;
                accessible = *acc == 'Y';
            } else {
                accessible = 0;
            }
            flags = entry->flags;
        } else {
            flags = entry->flags;
            accessible = (flags & 4) != 0;
        }
        if (type == base || (type->name == base->name && type->name != NULL)) {
            ambiguous = (flags & 8) != 0;
            if (!ambiguous && accessible) {
                found = 1;
                if (objBase != NULL) {
                    if (flags & 1) {
                        *outPtr = *(void **)sub;
                    } else {
                        *outPtr = sub;
                    }
                    flags = entry->flags;
                }
            }
        }
        entry++;
    } while (!(ambiguous | found) && !(flags & 2));

    if (ambiguous | found) {
        return found;
    }

    /* recurse into the bases' own bases */
    entry = derived->bases;
    do {
        void *sub = NULL;
        void *subOut;
        const CxxTypeInfo *type = entry->type;
        int recurse;

        if (objBase != NULL) {
            sub = objBase + entry->offset;
            if (entry->flags & 1) {
                sub = *(void **)sub;
            }
        }
        if (useAccess != 0) {
            recurse = 1;
        } else {
            recurse = 0;
            if ((entry->flags & 4) && !(entry->flags & 8)) {
                recurse = 1;
            }
        }
        if (type->bases != NULL && recurse &&
            CxxFindBaseClass(&sub, &subOut, type, base, access, useAccess) != 0) {
            if (objBase != NULL) {
                *outPtr = subOut;
            }
            return 1;
        }
        flags = entry->flags;
        entry++;
    } while (!(flags & 2));
    return found;
}
