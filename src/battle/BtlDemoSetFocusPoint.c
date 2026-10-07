// bdc 0x088ff350 BtlDemoSetFocusPoint
#include "bdc.h"

/* Stores a point (focusPos) and a value (focusValue) in the battle intro demo task (task id 0x65,
   0x790 bytes, vtable `0x08af45fc`, `BtlDemoCtor`); used by Bakugan state 15 through
   `BtlDemoSetFocusPointIfActive`. The original copies the 4-float point with one VFPU
   `lv.q`/`sv.q` pair. */

void BtlDemoSetFocusPoint(BtlDemo *demo, int value, float *pos)
{
    int i;

    for (i = 0; i < 4; i++) {
        demo->focusPos[i] = pos[i];
    }
    demo->focusValue = value;
}
