// bdc 0x08a24bec GmoPaletteArrayMeasureCopy
#include "bdc.h"

/* Measure pass for copying `count` palette images (0x30-byte records): when `flags & 0x21` reserves
   the records (`GmoImagePlanReserveTracks`) and measures each (`GmoPaletteMeasureCopyOne`).
   Returns 1 (0 for NULL arguments). */

s32 GmoPaletteArrayMeasureCopy(void *textures, s32 count, u32 flags, void *arena)
{
    GmoImage *img = (GmoImage *)textures;
    s32 i;

    if (img == NULL || arena == NULL) {
        return 0;
    }
    if ((flags & 0x21) != 0) {
        GmoImagePlanReserveTracks(count, arena);
        for (i = 0; i < count; i++) {
            GmoPaletteMeasureCopyOne(NULL, img, flags, arena);
            img++;
        }
    }
    return 1;
}
