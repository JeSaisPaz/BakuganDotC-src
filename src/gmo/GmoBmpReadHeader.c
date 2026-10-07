// bdc 0x08a280d8 GmoBmpReadHeader
#include "bdc.h"

/* Unpacks the 14-byte BMP file header and the 40-byte BITMAPINFOHEADER from `data` into the aligned
   struct `hdr` (type, file size, reserved, data offset, header size, width, height, planes, bit
   count, compression, image size, resolution, colours used/important). */

void GmoBmpReadHeader(const void *data, void *hdr)
{
  GmoBmpHeader *h = (GmoBmpHeader *)hdr;
  const u16 *p16;
  const u32 *p32;

  p16 = GmoBmpReadU16((const u16 *)data, &h->type);
  p32 = GmoBmpReadU32((const u32 *)p16, &h->fileSize);
  p16 = GmoBmpReadU16((const u16 *)p32, &h->reserved1);
  p16 = GmoBmpReadU16(p16, &h->reserved2);
  p32 = GmoBmpReadU32((const u32 *)p16, &h->dataOffset);
  p32 = GmoBmpReadU32(p32, &h->headerSize);
  p32 = GmoBmpReadU32(p32, (u32 *)&h->width);
  p32 = GmoBmpReadU32(p32, (u32 *)&h->height);
  p16 = GmoBmpReadU16((const u16 *)p32, &h->planes);
  p16 = GmoBmpReadU16(p16, &h->bitCount);
  p32 = GmoBmpReadU32((const u32 *)p16, &h->compression);
  p32 = GmoBmpReadU32(p32, &h->imageSize);
  p32 = GmoBmpReadU32(p32, (u32 *)&h->xPelsPerMeter);
  p32 = GmoBmpReadU32(p32, (u32 *)&h->yPelsPerMeter);
  p32 = GmoBmpReadU32(p32, &h->colorsUsed);
  GmoBmpReadU32(p32, &h->colorsImportant);
}
