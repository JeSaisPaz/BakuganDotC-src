// bdc 0x089f7cb8 GfxTextureBuildDl
#include "bdc.h"

/* Builds the GE command block of a texture object (`blocks`, also stored in `curBlock`/`gmo.image`):
   TMODE (`0xc2`, mip level count from the TIM2 picture header clamped to 4, swizzle bit from
   `gsTexClut` bit 0) and TPSM (`0xc3`, `GfxTextureGetPsm`); the pixel data (`unk11c` =
   picture + headerSize) is copied to VRAM when `g_gfxTexVramEnabled` is set and a slot or pool
   block is free. Then per mip level TBW/TBP/TSIZE (level byte size = `width*height` rounded down to
   powers of two, x2 for 16-bit/T16, x4 for 32-bit/T32, /2 for T4, then /4 per level), TWRAP, TFLT
   (plus TSLOPE and TEXLEVEL when there are >= 2 levels) and, for CLUT pictures, TFLUSH/CMODE/CBP/
   CLOAD/RET (the CLUT is unswizzled once, its alpha doubled once, and copied to VRAM when possible);
   without a CLUT just TFLUSH/RET. `mipCmdIdx` gets the TFLUSH word index. Unless `singleSlot` is set
   the first 0x60-byte block is copied into slots 1..7 with each slot's CBP pointing at the next
   palette (0x40 bytes apart, 0x400 for T8). Ends with a dcache writeback of the 0x300-byte block
   array. Called by `GfxTextureInitFromTim2`, `GfxTextureRebuild` and `GfxTextureReleaseVram`. */

/* floor(log2(n)) as the asm's `31 - clz(n)` (Allegrex clz(0) == 32). */
static s32 Log2Floor(s32 n)
{
  return 31 - ((n != 0) ? __builtin_clz((u32)n) : 32);
}

void GfxTextureBuildDl(void *tex, s32 width, s32 height)
{
  GfxTexture *t = (GfxTexture *)tex;
  GfxTim2Picture *pic;
  u8 *blk;
  s32 levels;
  s32 wLog;
  s32 hLog;
  s32 size;
  s32 total;
  s32 levelSize;
  s32 i;
  s32 cmdIdx;
  s32 bias;
  s32 clamped;
  u32 psm;
  u32 clutPsm;
  u32 flags;
  u32 magFlt;
  u32 minFlt;
  s32 clutFmt;
  s32 clutColors;
  s32 clutBytes;
  s32 clutStride;
  s32 clutOff;
  s32 slot;
  u8 single;
  u32 addr;
  void *vram;
  u8 *alpha;
  u32 a;

  blk = t->blocks;
  t->curBlock = blk;
  pic = t->picture;
  t->gmo.image = blk;
  levels = pic->mipMapTextures;
  if (levels > 4) {
    pic->mipMapTextures = 4;
    pic = t->picture;
    blk = t->blocks;
    levels = pic->mipMapTextures;
  }
  ((u32 *)blk)[0] = (u32)(levels - 1) << 16 | 0xc2000000 | (pic->gsTexClut & 1);
  psm = GfxTextureGetPsm(t);
  ((u32 *)t->blocks)[1] = psm | 0xc3000000;

  wLog = Log2Floor(width);
  hLog = Log2Floor(height);
  size = (1 << (wLog & 0x1f)) * (1 << (hLog & 0x1f));
  t->unk11c = (u8 *)t->picture + t->picture->headerSize;

  switch (GfxTextureGetPsm(t)) {
  case 0:
  case 1:
  case 2:
  case 6:
    size = size * 2;
    break;
  case 3:
  case 7:
    size = size * 4;
    break;
  case 4:
    size = size / 2;
    break;
  case 5:
  default:
    break;
  }

  if (g_gfxTexVramEnabled != 0) {
    /* the whole mip chain: each level is a quarter of the previous one */
    total = size;
    levelSize = size / 4;
    levels = t->picture->mipMapTextures;
    for (i = 1; i < levels; i++) {
      total += levelSize;
      levelSize = levelSize / 4;
    }
    vram = GfxDisplayVramAllocSlot(g_gfxDisplay, total);
    if (vram == NULL) {
      vram = GfxDisplayVramAlloc(g_gfxDisplay, total, 0, 0);
    }
    if (vram != NULL) {
      memcpy(vram, t->unk11c, total);
      t->unk11c = vram;
      t->vramBlock0 = vram;
    }
  }

  /* per mip level: TBW (with the address high bits), TBP, TSIZE */
  addr = PspAddr(t->unk11c);
  pic = t->picture;
  cmdIdx = 2;
  for (i = 0; i < pic->mipMapTextures; i++) {
    ((u32 *)t->blocks)[cmdIdx] = (u32)(i + 0xa8) << 24 | (addr >> 24 & 0xf) << 16 |
                                 (u32)(1 << (wLog & 0x1f));
    ((u32 *)t->blocks)[cmdIdx + 1] = (u32)(i + 0xa0) << 24 | (addr & 0xffffff);
    ((u32 *)t->blocks)[cmdIdx + 2] = (u32)(i + 0xb8) << 24 | (u32)hLog << 8 | (u32)wLog;
    addr += size;
    pic = t->picture;
    wLog--;
    hLog--;
    size = size / 4;
    cmdIdx += 3;
  }

  /* TWRAP */
  ((u32 *)t->blocks)[cmdIdx] = (u32)pic->gsTex1[2] << 8 | 0xc7000000 | pic->gsTex1[1];
  cmdIdx++;
  pic = t->picture;
  if (i >= 2) {
    /* mipmapped TFLT: filter codes >= 2 move up to the GE mipmap variants */
    magFlt = pic->gsTex1[6];
    minFlt = pic->gsTex1[7];
    if (minFlt >= 2) {
      minFlt += 2;
    }
    if (magFlt >= 2) {
      magFlt += 2;
    }
    ((u32 *)t->blocks)[cmdIdx] = magFlt << 8 | 0xc6000000 | minFlt;
    ((u32 *)t->blocks)[cmdIdx + 1] = *(u32 *)&t->picture->mipSlope >> 8 | 0xd0000000;
    pic = t->picture;
    bias = (s32)(pic->mipBias * 16.0f);
    if (bias > 0x7f) {
      clamped = 0x7f;
    } else {
      clamped = bias;
      if (bias < -0x80) {
        clamped = -0x80;
      }
    }
    ((u32 *)t->blocks)[cmdIdx + 2] = ((u32)clamped & 0xff) << 16 | 0xc8000000 | pic->gsTex1[5];
    cmdIdx += 3;
  } else {
    ((u32 *)t->blocks)[cmdIdx] = (u32)pic->gsTex1[3] << 8 | 0xc6000000 | pic->gsTex1[4];
    cmdIdx++;
  }

  clutStride = 0x40;
  psm = GfxTextureGetPsm(t);
  pic = t->picture;
  if (psm == 5) {
    clutStride = 0x400;
  }
  clutFmt = (s8)pic->clutType & 7;
  if (clutFmt != 0) {
    clutPsm = g_gfxPsmTable[clutFmt];
    clutColors = (s16)pic->clutColors;
    t->clutData = (u8 *)pic + pic->headerSize + pic->imageSize;
    clutBytes = clutColors * 2;
    if ((pic->clutType & 0x80) == 0) {
      GfxTextureUnswizzleRows(t->clutData, clutPsm, clutColors);
      t->picture->clutType |= 0x80;
      pic = t->picture;
    }
    flags = pic->gsTexClut;
    if ((flags & 2) == 0) {
      /* PS2 alpha 0x80 = opaque: double it to the PSP range, saturating at 0xff */
      for (i = 0; i < clutColors; i++) {
        alpha = &((u8 (*)[4])t->clutData)[i][3]; /* RGBA8888 entry, byte 3 = alpha */
        a = *alpha * 2;
        if (a >= 0x100) {
          a = 0xff;
        }
        *alpha = (u8)a;
      }
      t->picture->gsTexClut |= 2;
    }
    if (clutPsm == 3) {
      clutBytes = clutBytes * 2;
    }
    vram = GfxDisplayVramAllocSlot(g_gfxDisplay, clutBytes);
    if (g_gfxTexVramEnabled != 0 && vram == NULL) {
      vram = GfxDisplayVramAlloc(g_gfxDisplay, clutBytes, 0, 0);
    }
    if (vram != NULL) {
      memcpy(vram, t->clutData, clutBytes);
      t->clutData = vram;
      t->vramBlock1 = vram;
    }
    ((u32 *)t->blocks)[cmdIdx] = 0xcb000000;
    ((u32 *)t->blocks)[cmdIdx + 1] = clutPsm | 0xc500ff00;
    ((u32 *)t->blocks)[cmdIdx + 2] = (PspAddr(t->clutData) >> 24 & 0xf) << 16 | 0xb1000000;
    ((u32 *)t->blocks)[cmdIdx + 3] = (PspAddr(t->clutData) & 0xffffff) | 0xb0000000;
    ((u32 *)t->blocks)[cmdIdx + 4] = (u32)(clutStride / 32) | 0xc4000000;
    ((u32 *)t->blocks)[cmdIdx + 5] = 0x0b000000;
  } else {
    ((u32 *)t->blocks)[cmdIdx] = 0xcb000000;
    ((u32 *)t->blocks)[cmdIdx + 1] = 0x0b000000;
  }
  blk = t->blocks;
  single = t->singleSlot;
  t->mipCmdIdx = cmdIdx;

  if (single == 0) {
    /* slots 1..7: copies of slot 0, each pointing CBPH/CBP at the next palette */
    clutOff = clutStride;
    for (slot = 1; slot < 8; slot++) {
      memcpy(blk + slot * 0x60, blk, 0x60);
      ((u32 *)t->blocks)[slot * 0x18 + cmdIdx + 2] =
          ((PspAddr(t->clutData) + clutOff) >> 24 & 0xf) << 16 | 0xb1000000;
      ((u32 *)t->blocks)[slot * 0x18 + cmdIdx + 3] =
          ((PspAddr(t->clutData) + clutOff) & 0xffffff) | 0xb0000000;
      clutOff += clutStride;
      blk = t->blocks;
    }
  }
  sceKernelDcacheWritebackRange(blk, 0x300);
}
