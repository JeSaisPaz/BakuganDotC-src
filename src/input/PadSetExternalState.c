// bdc 0x089ce05c PadSetExternalState
#include "bdc.h"

/* Overwrites a `PadState`'s `buttons`, `pressed`, `released` and `repeat` masks with four `u16`s
   from `masks` and its stick (`stickX`, `stickY`) with two floats from `stick`; a NULL argument
   clears the corresponding fields. Lets a pad object replay input that did not come from `sceCtrl`
   (the remote player's input in net play). */

void PadSetExternalState(PadState *pad, u16 *masks, float *stick)
{
    if (masks != NULL) {
        /* inlined 2-byte memcpys: copied byte by byte, `masks` need not be aligned */
        memcpy(&pad->buttons, &masks[0], 2);
        memcpy(&pad->pressed, &masks[1], 2);
        memcpy(&pad->released, &masks[2], 2);
        memcpy(&pad->repeat, &masks[3], 2);
    } else {
        memset(&pad->buttons, 0, 2);
        memset(&pad->pressed, 0, 2);
        memset(&pad->released, 0, 2);
        memset(&pad->repeat, 0, 2);
    }
    if (stick != NULL) {
        pad->stickX = stick[0];
        pad->stickY = stick[1];
    } else {
        pad->stickX = 0.0f;
        pad->stickY = 0.0f;
    }
}
