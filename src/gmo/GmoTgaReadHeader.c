// bdc 0x08a277d8 GmoTgaReadHeader
#include "bdc.h"

/* Unpacks the 18-byte TGA header from `data` into the aligned struct `hdr` (id length, colour-map
   type, image type, colour-map first/length/depth, x/y origin, width, height, pixel depth,
   descriptor) and returns the pointer to the data after the header and the image-ID field. */

const u8 *GmoTgaReadHeader(const u8 *data, GmoTgaHeader *hdr)
{
    const u8 *p8;
    const u16 *p16;

    p8 = GmoTgaReadU8(data, &hdr->idLength);
    p8 = GmoTgaReadU8(p8, &hdr->colorMapType);
    p8 = GmoTgaReadU8(p8, &hdr->imageType);
    p16 = GmoTgaReadU16((const u16 *)p8, &hdr->cmapFirst);
    p16 = GmoTgaReadU16(p16, &hdr->cmapLength);
    p8 = GmoTgaReadU8((const u8 *)p16, &hdr->cmapDepth);
    p16 = GmoTgaReadU16((const u16 *)p8, &hdr->xOrigin);
    p16 = GmoTgaReadU16(p16, &hdr->yOrigin);
    p16 = GmoTgaReadU16(p16, &hdr->width);
    p16 = GmoTgaReadU16(p16, &hdr->height);
    p8 = GmoTgaReadU8((const u8 *)p16, &hdr->pixelDepth);
    p8 = GmoTgaReadU8(p8, &hdr->descriptor);
    return p8 + hdr->idLength;
}
