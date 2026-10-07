// bdc 0x08904b80 BtlDemoScbObjectKind
#include "bdc.h"

/* Classifies a `.scb` scene object id (signed compares): 0x29..0x48 and 0x2d7..0x2f6 → 1,
   0x49..0x88 → 2, 0x89..0x150 → 3, 0x181..0x1e4 → 4, anything else 0. `obj` is unused. */
int BtlDemoScbObjectKind(void *obj, int id)
{
    (void)obj;
    if (id > 0x28 && id < 0x49) {
        return 1;
    }
    if (id > 0x2d6 && id < 0x2f7) {
        return 1;
    }
    if (id > 0x48 && id < 0x89) {
        return 2;
    }
    if (id > 0x88 && id < 0x151) {
        return 3;
    }
    if (id > 0x180 && id < 0x1e5) {
        return 4;
    }
    return 0;
}
