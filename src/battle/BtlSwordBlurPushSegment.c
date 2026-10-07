// bdc 0x0882a2e0 BtlSwordBlurPushSegment
#include "bdc.h"

/* Appends one blade segment (`tip`, `base`, vec4s) to the 8-entry ring buffers of the
   `BtlSwordBlur` trail: advances `head` (stored once as head+1, then again wrapped with `& 7`)
   and copies the two points into that slot. The first push after creation (`head == -1`) sets
   `head` to 0 and fills all 8 slots so the trail starts collapsed. The VFPU `lv.q`/`sv.q` pairs
   are plain 16-byte copies; no VFPU value is read by the caller. */
void BtlSwordBlurPushSegment(BtlSwordBlur *blur, const float *tip, const float *base)
{
    s32 i;

    if (blur->head == -1) {
        blur->head = 0;
        for (i = 0; i < 8; i++) {
            blur->tip[i][0] = tip[0];
            blur->tip[i][1] = tip[1];
            blur->tip[i][2] = tip[2];
            blur->tip[i][3] = tip[3];
            blur->base[i][0] = base[0];
            blur->base[i][1] = base[1];
            blur->base[i][2] = base[2];
            blur->base[i][3] = base[3];
        }
        return;
    }
    blur->head = blur->head + 1;
    blur->head = blur->head & 7;
    blur->tip[blur->head][0] = tip[0];
    blur->tip[blur->head][1] = tip[1];
    blur->tip[blur->head][2] = tip[2];
    blur->tip[blur->head][3] = tip[3];
    blur->base[blur->head][0] = base[0];
    blur->base[blur->head][1] = base[1];
    blur->base[blur->head][2] = base[2];
    blur->base[blur->head][3] = base[3];
}
