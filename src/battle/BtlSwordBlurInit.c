// bdc 0x0882a22c BtlSwordBlurInit
#include "bdc.h"

/* Initialises the 0x140-byte sword-blur trail of a Bakugan's weapon bone: stores the owner and
   bone, marks the ring buffers empty (head -1), copies white (`g_colorWhite`) into the colour
   (a VFPU quad copy), then zeroes the alpha and the active flag and looks up the "swordblur"
   texture (`GfxFindTexture`). Returns `blur`. */
BtlSwordBlur *BtlSwordBlurInit(BtlSwordBlur *blur, BtlBakugan *bakugan, void *bone)
{
    blur->owner = bakugan;
    blur->bone = bone;
    blur->head = -1;
    blur->color[0] = g_colorWhite.x;
    blur->color[1] = g_colorWhite.y;
    blur->color[2] = g_colorWhite.z;
    blur->color[3] = g_colorWhite.w;
    blur->color[3] = 0.0f;
    blur->active = 0;
    blur->texture = GfxFindTexture("swordblur");
    return blur;
}
