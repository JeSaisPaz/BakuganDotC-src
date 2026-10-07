// bdc 0x088ff364 BtlDemoSetFocusPointIfActive
#include "bdc.h"

/* Finds the battle intro demo task (id 0x65) and, when it exists, calls `BtlDemoSetFocusPoint` on
   it with a stack copy of the 4-float position `pos`. */
void BtlDemoSetFocusPointIfActive(int value, float *pos)
{
    BtlDemo *demo = CoreTaskFind(0x65);
    float copy[4] __attribute__((aligned(16)));

    if (demo != NULL) {
        copy[0] = pos[0];
        copy[1] = pos[1];
        copy[2] = pos[2];
        copy[3] = pos[3];
        BtlDemoSetFocusPoint(demo, value, copy);
    }
}
