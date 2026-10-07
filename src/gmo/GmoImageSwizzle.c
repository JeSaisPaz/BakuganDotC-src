// bdc 0x08a122e0 GmoImageSwizzle
#include "bdc.h"

#define SWIZZLE_BLOCK_BYTES 0x80 /* one 16-byte x 8-row block */

/* Copies pixel data between two image buffers, each either linear (`pitch` bytes per row) or in the
   GE swizzled layout (16-byte by 8-row blocks), in 16-byte units. The copied area is the smaller of
   the two pitches and heights. */

void GmoImageSwizzle(void *dst, int dstPitch, int dstHeight, int dstSwizzled, const void *src,
                     int srcPitch, int srcHeight, int srcSwizzled)

{
  u8 *d;
  const u8 *s;
  u32 *out;
  const u32 *in;
  int cols;
  int rows;
  int rowStep;
  int colStep;
  int off;
  int r;
  int x;

  d = (u8 *)dst;
  s = (const u8 *)src;
  if (srcHeight < dstHeight) {
    dstHeight = srcHeight;
  }
  cols = dstPitch;
  if (srcPitch < dstPitch) {
    cols = srcPitch;
  }
  if (dstSwizzled == 0) {
    /* linear destination: row by row, 16-byte column steps through the source */
    colStep = 0x10;
    rowStep = srcPitch;
    if (srcSwizzled != 0) {
      colStep = SWIZZLE_BLOCK_BYTES;
      rowStep = 0x10;
    }
    while (dstHeight > 0) {
      rows = dstHeight;
      if (rows > 8) {
        rows = 8;
      }
      for (r = 0; r < rows; r++) {
        out = (u32 *)(d + r * dstPitch);
        in = (const u32 *)(s + r * rowStep);
        for (x = 0; x < cols; x += 0x10) {
          out[0] = in[0];
          out[1] = in[1];
          out[2] = in[2];
          out[3] = in[3];
          in = (const u32 *)((const u8 *)in + colStep);
          out += 4;
        }
      }
      dstHeight -= 8;
      d += dstPitch * 8;
      s += srcPitch * 8;
    }
  }
  else {
    /* swizzled destination: block by block, 8 rows of 16 bytes each */
    rowStep = srcPitch;
    colStep = 1;
    if (srcSwizzled != 0) {
      rowStep = 0x10;
      colStep = 8;
    }
    while (dstHeight > 0) {
      rows = dstHeight;
      if (rows > 8) {
        rows = 8;
      }
      off = 0;
      for (x = 0; x < cols; x += 0x10) {
        out = (u32 *)(d + (x / 0x10) * SWIZZLE_BLOCK_BYTES);
        in = (const u32 *)(s + off);
        for (r = 0; r < rows; r++) {
          out[0] = in[0];
          out[1] = in[1];
          out[2] = in[2];
          out[3] = in[3];
          in = (const u32 *)((const u8 *)in + rowStep);
          out += 4;
        }
        off += colStep * 0x10;
      }
      dstHeight -= 8;
      d += dstPitch * 8;
      s += srcPitch * 8;
    }
  }
}
