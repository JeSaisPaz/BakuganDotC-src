// bdc 0x088ff73c BtlDemoDrawModels
#include "bdc.h"

/* Draws the demo model list `list` (`BtlDemoDrawModelList`), threading the display-list cursor `dl` through
   and returning the last call's result: normally in one pass (pass 0); with `twoPass` set in
   passes 2 then 1 with `GmoSetDepthWriteOverride` on, then switches the override off and
   resets the draw pass to 0 (`GfxSetModelDrawPass`). `demo` is unused. */

u32 *BtlDemoDrawModels(void *demo, u32 *dl, void **list, bool twoPass)
{
    u32 *result;

    (void)demo;
    if (twoPass) {
        GmoSetDepthWriteOverride(true);
        result = BtlDemoDrawModelList(dl, list, 2);
        result = BtlDemoDrawModelList(result, list, 1);
        GmoSetDepthWriteOverride(false);
        GfxSetModelDrawPass(0);
    } else {
        result = BtlDemoDrawModelList(dl, list, 0);
    }
    return result;
}
