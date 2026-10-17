// bdc 0x0884b324 BtlDrawModelList
#include "bdc.h"

/* Draws a model list in passes 2 then 1 (`BtlDrawModelListPass`) with the GMO depth-write
   override enabled around both passes (`GmoSetDepthWriteOverride`), then resets the model draw
   pass to 0 (`GfxSetModelDrawPass`); returns the advanced packet pointer. Used by
   `BtlMainDrawScene`. */

u32 *BtlDrawModelList(void *main, u32 *packet, void **list)
{
    (void)main;
    GmoSetDepthWriteOverride(true);
    packet = BtlDrawModelListPass(packet, list, 2);
    packet = BtlDrawModelListPass(packet, list, 1);
    GmoSetDepthWriteOverride(false);
    GfxSetModelDrawPass(0);
    return packet;
}
