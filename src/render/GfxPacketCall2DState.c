// bdc 0x089f1428 GfxPacketCall2DState
#include "bdc.h"

/* Opens a chunk on `packet`, writes the 2D render-state call (`GfxDlCall2DState`) and closes it. */
void GfxPacketCall2DState(void *packet)
{
    u32 *list;

    list = GfxPacketBeginChunk(packet);
    list = GfxDlCall2DState(list);
    GfxPacketEndChunk(packet, list);
}
