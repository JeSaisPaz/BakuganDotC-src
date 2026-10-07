// bdc 0x08a11184 GmoTextureWriteDl
#include "bdc.h"

/* Writes the GE texture state of a `GmoTexture` into a display list at `*dl` (room: `*end`
   bytes). With `flags & 1` the texture binding: for a NULL texture a BASE+CALL of the 1x1 white
   block `g_gmoWhiteTexDl` (TBP0/TBW0 filled and written back on first use); with bit 31 clear
   and a prebuilt image list (`image`) a BASE+CALL of it; otherwise TMODE/TPSM, TBP/TBW/TSIZE per
   level (or the two frames of the active track when `trackFlags & 1`), TFLUSH, then the CLUT load
   (CBP/CBW/CLOAD, CMODE) of the palette. With `flags & 2` the per-texture state selected by
   `trackFlags`: TBIAS, UV offset/scale, blend preset, TFUNC, TFLT, TWRAP. A command is written
   only when it fits; with no list (`dl` or `*dl` NULL) nothing is written and the size is only
   measured. Returns the byte count (0 when the list overflowed); `*dl` is advanced and `*end`
   reduced by it. */

/* GE address bits 24..27 (BASE / TBW / CBW high bits) and 0..23 of a pointer. */
#define GE_ADDR_HI(p) ((u32)(((uintptr_t)(p) >> 24) & 0xf) << 16)
#define GE_ADDR_LO(p) ((u32)((uintptr_t)(p) & 0xffffff))

/* ceil(log2(n)) as `32 - clz(n - 1)`, with the Allegrex clz(0) == 32. */
static inline u32 GmoTexLog2(u32 n)
{
  u32 v = n - 1;
  return 32 - ((v != 0) ? (u32)__builtin_clz(v) : 32);
}

/* Active animation track: `lastTrack`, or `trackCount` when it is negative or past it. */
static inline GmoTexTrack *GmoTexActiveTrack(GmoTexture *tex)
{
  s32 idx = (s8)tex->lastTrack;

  if (idx < 0) {
    idx = tex->trackCount;
  } else if ((s32)tex->trackCount < idx) {
    idx = tex->trackCount;
  }
  return (GmoTexTrack *)tex->tracks + idx;
}

int GmoTextureWriteDl(void *texp, u32 **dl, u32 *end, u32 flags)
{
  GmoTexture *tex = (GmoTexture *)texp;
  u32 *start = NULL;
  u32 *limit = NULL;
  u32 *p;
  u32 *q;
  GmoImage *img;
  GmoTexTrack *track;
  s32 cur;
  s32 next;
  s32 n;

  if (dl != NULL) {
    start = *dl;
    if (start != NULL) {
      limit = (u32 *)~(uintptr_t)3;
      if (end != NULL) {
        limit = (u32 *)((u8 *)start + (*end & ~3u));
      }
    }
  }
  p = start;

  if ((flags & 1) != 0) {
    if (tex == NULL) {
      if (g_gmoWhiteTexDl[3] == 0) {
        g_gmoWhiteTexDl[3] = GE_ADDR_LO(g_gmoWhiteTexDl) | 0xa0000000;
        g_gmoWhiteTexDl[4] = GE_ADDR_HI(g_gmoWhiteTexDl) | 0xa8000008;
        sceKernelDcacheWritebackRange(g_gmoWhiteTexDl, 0x40);
      }
      p = start + 2;
      if (p <= limit) {
        start[0] = GE_ADDR_HI(&g_gmoWhiteTexDl[1]) | 0x10000000;
        start[1] = GE_ADDR_LO(&g_gmoWhiteTexDl[1]) | 0x0a000000;
      }
    } else if ((s32)flags >= 0 && tex->image != NULL) {
      p = start + 2;
      if (p <= limit) {
        start[0] = GE_ADDR_HI(tex->image) | 0x10000000;
        start[1] = GE_ADDR_LO(tex->image) | 0x0a000000;
      }
    } else if (tex->flags == 0) {
      /* Static texture: level 0 of the first image, CLUT of the first palette. */
      q = start;
      img = GmoTextureFindImage(tex, 1, 0);
      if (img != NULL || (img = tex->images) != NULL) {
        q = start + 6;
        if (q <= limit) {
          void *lv;
          u32 align = img->widthAlign - 1;

          start[1] = img->format | 0xc3000000;
          start[0] = img->flags16 | 0xc2000000;
          lv = img->levels[0];
          start[2] = GE_ADDR_LO(lv) | 0xa0000000;
          start[3] = GE_ADDR_HI(lv) | 0xa8000000 | ((img->width + align) & ~align);
          start[4] = GmoTexLog2(img->height) << 8 | 0xb8000000 | GmoTexLog2(img->width);
          start[5] = 0xcb000000;
        }
      }
      p = q;
      img = GmoTextureFindPalette(tex, 1, 0);
      if (img != NULL || (img = tex->palettes) != NULL) {
        p = q + 4;
        if (p <= limit) {
          void *lv;

          q[0] = (u32)tex->frameC << 16 | (u32)tex->frameB << 8 | (u32)tex->frameA << 2 |
                 img->format | 0xc5000000;
          lv = img->levels[0];
          q[3] = img->flags1a | 0xc4000000;
          q[1] = GE_ADDR_LO(lv) | 0xb0000000;
          q[2] = GE_ADDR_HI(lv) | 0xb1000000;
        }
      }
    } else {
      /* Animated texture: frames picked by the active track. */
      q = start;
      img = GmoTextureFindImage(tex, 1, 0);
      if (img != NULL || (img = tex->images) != NULL) {
        u32 align = img->widthAlign - 1;
        s32 w = img->width;
        s32 h = img->height;
        s32 levels = img->levelCount;
        s32 frames = img->frameCount;

        track = GmoTexActiveTrack(tex);
        cur = 0;
        next = 0;
        if (track != NULL) {
          next = track->frameB0 % frames;
          cur = track->frameA0 % frames;
          if (next < 0) {
            next += frames;
          }
          if (cur < 0) {
            cur += frames;
          }
        }
        if ((tex->trackFlags & 1) != 0) {
          /* Two single-level frames as texture levels 0 and 1. */
          q = start + 9;
          if (q <= limit) {
            void *lv0 = img->levels[cur * levels];
            void *lv1 = img->levels[next * levels];
            u32 bw = (w + align) & ~align;
            u32 size = GmoTexLog2(h) << 8 | GmoTexLog2(w);

            start[0] = ((tex->flags >> 3) & 1) << 8 | 0x10000 | img->flags16 | 0xc2000000;
            start[1] = img->format | 0xc3000000;
            start[2] = GE_ADDR_LO(lv0) | 0xa0000000;
            start[3] = GE_ADDR_HI(lv0) | 0xa8000000 | bw;
            start[4] = size | 0xb8000000;
            start[5] = GE_ADDR_LO(lv1) | 0xa1000000;
            start[6] = bw | GE_ADDR_HI(lv1) | 0xa9000000;
            start[7] = size | 0xb9000000;
            start[8] = 0xcb000000;
          }
        } else {
          /* All mip levels of the current frame. */
          q = start + levels * 3 + 3;
          if (q <= limit) {
            void **lv = img->levels + cur * levels;
            u32 *out = start + 2;

            start[0] = (u32)(levels - 1) << 16 | ((tex->flags >> 2) & 1) << 8 | img->flags16 |
                       0xc2000000;
            start[1] = img->format | 0xc3000000;
            for (n = 0; n < levels; n++) {
              out[0] = (u32)(n + 0xa0) << 24 | GE_ADDR_LO(*lv);
              out[1] = (u32)(n + 0xa8) << 24 | GE_ADDR_HI(*lv) | ((w + align) & ~align);
              out[2] = (u32)(n + 0xb8) << 24 | GmoTexLog2(h) << 8 | GmoTexLog2(w);
              lv++;
              out += 3;
              if (img->mipmapMode == 1) {
                w = (w + 1) / 2;
                h = (h + 1) / 2;
              }
            }
            out[0] = 0xcb000000;
          }
        }
      }

      p = q;
      img = GmoTextureFindPalette(tex, 1, 0);
      if (img != NULL || (img = tex->palettes) != NULL) {
        s32 levels = img->levelCount;
        s32 frames = img->frameCount;
        u32 *mode = NULL;

        track = GmoTexActiveTrack(tex);
        cur = 0;
        next = 0;
        if (track != NULL) {
          next = track->frameB1 % frames;
          cur = track->frameA1 % frames;
          if (next < 0) {
            next += frames;
          }
          if (cur < 0) {
            cur += frames;
          }
        }
        if ((tex->trackFlags & 1) != 0) {
          p = q + 7;
          if (p <= limit) {
            s32 blocks = img->width >> 4;

            if (img->format == 3) {
              blocks <<= 1;
            }
            if (next == cur + 1 && levels == 1 && blocks < 0x11) {
              /* Adjacent frames: one CLUT load, three zero words keep the size fixed. */
              void *lv = img->levels[cur];

              q[0] = GE_ADDR_LO(lv) | 0xb0000000;
              q[1] = GE_ADDR_HI(lv) | 0xb1000000;
              q[2] = (u32)blocks << 1 | 0xc4000000;
              for (n = 3; n < 6; n++) {
                q[n] = 0;
              }
            } else {
              u8 *lvNext = (u8 *)img->levels[next * levels] - blocks * 0x20;
              void *lvCur = img->levels[cur * levels];

              q[0] = GE_ADDR_LO(lvNext) | 0xb0000000;
              q[1] = GE_ADDR_HI(lvNext) | 0xb1000000;
              q[2] = (u32)blocks << 1 | 0xc4000000;
              q[3] = GE_ADDR_LO(lvCur) | 0xb0000000;
              q[4] = GE_ADDR_HI(lvCur) | 0xb1000000;
              q[5] = (u32)blocks | 0xc4000000;
            }
            mode = q + 6;
          }
        } else {
          p = q + 4;
          if (p <= limit) {
            void *lv = img->levels[cur * levels];

            q[0] = GE_ADDR_LO(lv) | 0xb0000000;
            q[1] = GE_ADDR_HI(lv) | 0xb1000000;
            q[2] = img->flags1a | 0xc4000000;
            mode = q + 3;
          }
        }
        if (mode != NULL) {
          *mode = (u32)tex->frameC << 16 | (u32)tex->frameB << 8 | (u32)tex->frameA << 2 |
                  img->format | 0xc5000000;
        }
      }
    }
  }

  if ((flags & 2) != 0 && tex != NULL && tex->trackFlags != 0) {
    u32 tf = tex->trackFlags;

    if ((tf & 2) != 0) {
      q = p + 1;
      if (q <= limit) {
        if ((tf & 1) == 0) {
          p[0] = 0xc8000000;
        } else {
          /* TBIAS: constant mode, bias = track blend in 4.4 fixed point, clamped to s8. */
          float bias = 0.0f;
          s32 b;
          u32 v;

          track = GmoTexActiveTrack(tex);
          if (track != NULL) {
            bias = track->blend * g_gmoTexBiasScale;
          }
          b = (s32)bias;
          v = 0x7f;
          if (b < 0x80) {
            v = (b < -0x80) ? 0x80 : ((u32)b & 0xff);
          }
          p[0] = v << 16 | 0xc8000001;
        }
      }
      p = q;
    }
    if ((tf & 4) != 0) {
      q = p + 4;
      if (q <= limit) {
        const float *uv = tex->uvTransform;
        union { float f; u32 u; } bits[4];

        if ((tex->flags & 0x20) == 0) {
          uv = g_gmoDefaultUvTransform;
        }
        bits[1].f = uv[1];
        bits[2].f = uv[2];
        bits[3].f = uv[3];
        bits[0].f = uv[0];
        p[1] = bits[1].u >> 8 | 0x4b000000;
        p[3] = bits[3].u >> 8 | 0x49000000;
        p[0] = bits[0].u >> 8 | 0x4a000000;
        p[2] = bits[2].u >> 8 | 0x48000000;
      }
      p = q;
    }
    if ((tf & 0x100) != 0) {
      q = p + 3;
      if (q <= limit) {
        const s8 *e = g_gmoBlendModeTable[tex->filterMin];

        p[2] = ((u32)(s32)e[3] & 0xffffff) | 0xe1000000;
        p[0] = (u32)(s32)e[0] << 8 | (u32)(s32)e[2] << 4 | (u32)(s32)e[1] | 0xdf000000;
        p[1] = 0xe0ffffff;
      }
      p = q;
    }
    if ((tf & 0x200) != 0) {
      q = p + 1;
      if (q <= limit) {
        p[0] = (u32)tex->filterMag << 8 | tex->filterMode | 0xc9000000;
      }
      p = q;
    }
    if ((tf & 0x400) != 0) {
      q = p + 1;
      if (q <= limit) {
        p[0] = (u32)tex->wrapU << 8 | tex->wrapV | 0xc6000000;
      }
      p = q;
    }
    if ((tf & 0x800) != 0) {
      q = p + 1;
      if (q <= limit) {
        p[0] = (u32)tex->flagsB << 8 | tex->flagsA | 0xc7000000;
      }
      p = q;
    }
  }

  if (dl != NULL) {
    if (limit < p) {
      return 0;
    }
    *dl = p;
  }
  if (end != NULL) {
    *end = (u32)((u8 *)limit - (u8 *)p);
  }
  return (int)((u8 *)p - (u8 *)start);
}
