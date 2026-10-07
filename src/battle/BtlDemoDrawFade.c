// bdc 0x088ffa4c BtlDemoDrawFade
#include "bdc.h"

/* State-0 draw of the battle intro demo task (`BtlDemo`, task id 0x65, vtable `0x08af45fc`,
   `BtlDemoCtor`): opens a render packet with sort key 1050 (`GfxNewRenderPacket`) and, when the
   fade colour's alpha `fadeColour.w` is not <= 0 (positive or NaN), draws the full-screen fade in that
   colour (`GfxPacketDrawScreenFlash`). */
void BtlDemoDrawFade(void *demo)
{
    BtlDemo *self = (BtlDemo *)demo;
    void *packet = GfxNewRenderPacket(1050.0f);

    if (!(self->fadeColour.w <= 0.0f)) {
        GfxPacketDrawScreenFlash(packet, &self->fadeColour);
    }
}
