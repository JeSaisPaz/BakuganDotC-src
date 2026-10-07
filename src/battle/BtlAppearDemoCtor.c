// bdc 0x088fff78 BtlAppearDemoCtor
#include "bdc.h"

/* Constructor of the Bakugan appear demo task: runs the battle intro demo
   constructor, installs the appear demo vtable, clears the three scene player
   slots and sets the fade colour to opaque black (0, 0, 0, alpha 1). */
CoreTask *BtlAppearDemoCtor(CoreTask *task, u32 arg)
{
    BtlAppearDemo *demo = (BtlAppearDemo *)task;

    BtlDemoCtor(task, arg);
    task->vtable = g_btlAppearDemoVtbl;
    demo->scenePlayers[2] = NULL;
    demo->scenePlayers[1] = NULL;
    demo->scenePlayers[0] = NULL;
    demo->base.fadeColour.x = 0.0f;
    demo->base.fadeColour.y = 0.0f;
    demo->base.fadeColour.z = 0.0f;
    demo->base.fadeColour.w = 1.0f;
    return task;
}
