// bdc 0x0885e9cc BtlBakuganInitMotions
#include "bdc.h"

/* Constructor helper of `BtlBakuganCtor`. Does nothing unless the motion manager exists and the
   kind (`base.base.unk08`) has a motion-name list (`g_charMotionNameLists`). Otherwise enables
   motion on the model, loads the kind's motion file (`g_charMotionFiles`,
   `GmoMotionLoadFile`) when its first motion is not registered, allocates (from the low heap)
   and fills the 0x129-entry motion-index table `motionTable` from the name list
   (`GmoMotionIndexOfName`, -1 for a NULL name). Then, unless the kind's bit in
   `g_btlKindSetupMask` is set, initialises the function-local static offset vectors and patches
   the shared motion data of kinds 2, 6, 9, 11, 13, 14, 17 and 18
   (`BtlMotionAddTrackOffsetByName`, `BtlMotionOffsetTrackKeysByName`,
   `BtlMotionSetTrackKeysByName`, `BtlMotionAddEndFrameByName`); for kind 14 (Hades) on stages
   4..7 it also swaps slots 0/1 of the `"10_D_Hades_Refrec"` texture. Only those kinds get their
   mask bit set. */

void BtlBakuganInitMotions(BtlBakugan *self)
{
  char **names;
  s32 index;
  bool fromLow;
  s16 *table;
  s16 value;
  int i;
  bool handled;

  if (!GmoMotionMgrExists()) {
    return;
  }
  GfxModelEnableMotion(&self->base);
  names = g_charMotionNameLists[self->base.base.unk08];
  if (names == NULL) {
    return;
  }
  index = GmoMotionIndexOfName(GmoMotionMgrGet(), names[0]);
  if (index == -1) {
    GmoMotionLoadFile(GmoMotionMgrGet(), g_charMotionFiles[self->base.base.unk08]);
  }

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  table = MemAlloc(0x129 * sizeof(s16), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->motionTable = table;

  for (i = 0; i < 0x129; i++) {
    value = -1;
    if (names[i] != NULL) {
      value = (s16)GmoMotionIndexOfName(GmoMotionMgrGet(), names[i]);
    }
    self->motionTable[i] = value;
  }

  if ((g_btlKindSetupMask & (1u << (self->base.base.unk08 & 0x1f))) != 0) {
    return;
  }

  if (g_btlKind9MotionOfsInit == 0) {
    g_btlKind9MotionOfsInit = 1;
    g_btlKind9MotionOfs[0] = 0.0f;
    g_btlKind9MotionOfs[1] = 80.0f;
    g_btlKind9MotionOfs[2] = 0.0f;
    g_btlKind9MotionOfs[3] = 0.0f;
  }
  if (g_btlMotionOfsY30Init == 0) {
    g_btlMotionOfsY30Init = 1;
    g_btlMotionOfsY30[0] = 0.0f;
    g_btlMotionOfsY30[1] = 30.0f;
    g_btlMotionOfsY30[2] = 0.0f;
    g_btlMotionOfsY30[3] = 0.0f;
  }
  if (g_btlKind13MotionOfsInit == 0) {
    g_btlKind13MotionOfsInit = 1;
    g_btlKind13MotionOfs[0] = 0.0f;
    g_btlKind13MotionOfs[1] = 25.0f;
    g_btlKind13MotionOfs[2] = 0.0f;
    g_btlKind13MotionOfs[3] = 0.0f;
  }
  if (g_btlKind17MotionOfsInit == 0) {
    g_btlKind17MotionOfsInit = 1;
    g_btlKind17MotionOfs[0] = 0.0f;
    g_btlKind17MotionOfs[1] = -50.0f;
    g_btlKind17MotionOfs[2] = 0.0f;
    g_btlKind17MotionOfs[3] = 0.0f;
  }
  if (g_btlKind6MotionOfsInit == 0) {
    g_btlKind6MotionOfsInit = 1;
    g_btlKind6MotionOfs[0] = 0.0f;
    g_btlKind6MotionOfs[1] = 25.0f;
    g_btlKind6MotionOfs[2] = 0.0f;
    g_btlKind6MotionOfs[3] = 0.0f;
  }
  if (g_btlKind11MotionOfsInit == 0) {
    g_btlKind11MotionOfsInit = 1;
    g_btlKind11MotionOfs[0] = 20.0f;
    g_btlKind11MotionOfs[1] = -25.0f;
    g_btlKind11MotionOfs[2] = -20.0f;
    g_btlKind11MotionOfs[3] = 0.0f;
  }
  if (g_btlKind14MotionOfsInit == 0) {
    g_btlKind14MotionOfsInit = 1;
    g_btlKind14MotionOfs[0] = 0.0f;
    g_btlKind14MotionOfs[1] = -36.0f;
    g_btlKind14MotionOfs[2] = 0.0f;
    g_btlKind14MotionOfs[3] = 0.0f;
  }

  handled = true;
  switch (self->base.base.unk08) {
  case 2:
    BtlMotionAddTrackOffsetByName(names[0x1a], 0, g_btlMotionOfsY30);
    break;
  case 6:
    BtlMotionAddEndFrameByName(-1.0f, names[0x112]);
    BtlMotionAddEndFrameByName(-1.0f, names[0x93]);
    BtlMotionAddTrackOffsetByName(names[0x93], 0, g_btlKind6MotionOfs);
    BtlMotionAddTrackOffsetByName(names[0x94], 0, g_btlKind6MotionOfs);
    BtlMotionOffsetTrackKeysByName(names[0x95], 0, g_btlKind6MotionOfs);
    break;
  case 9:
    BtlMotionSetTrackKeysByName(names[0x107], 0, g_btlKind9MotionOfs);
    BtlMotionSetTrackKeysByName(names[0x108], 0, g_btlKind9MotionOfs);
    BtlMotionSetTrackKeysByName(names[0x109], 0, g_btlKind9MotionOfs);
    break;
  case 11:
    BtlMotionAddTrackOffsetByName(names[0x10f], 0, g_btlKind11MotionOfs);
    break;
  case 13:
    BtlMotionAddTrackOffsetByName(names[0x1a], 0, g_btlKind13MotionOfs);
    BtlMotionAddEndFrameByName(-1.0f, names[0x93]);
    break;
  case 14:
    BtlMotionAddTrackOffsetByName(names[0x1a], 0, g_btlMotionOfsY30);
    if (GameStageIs4To7() != 0) {
      GfxTextureSwapSlots(GfxFindTexture("10_D_Hades_Refrec"), 0, 1);
    }
    BtlMotionAddTrackOffsetByName(names[0x10f], 0, g_btlKind14MotionOfs);
    break;
  case 17:
    BtlMotionAddTrackOffsetByName(names[0x107], 0, g_btlKind17MotionOfs);
    BtlMotionAddTrackOffsetByName(names[0x108], 0, g_btlKind17MotionOfs);
    BtlMotionAddTrackOffsetByName(names[0x109], 0, g_btlKind17MotionOfs);
    break;
  case 18:
    BtlMotionAddTrackOffsetByName(names[0x92], 0, g_btlMotionOfsY30);
    BtlMotionAddTrackOffsetByName(names[0x93], 0, g_btlMotionOfsY30);
    BtlMotionAddTrackOffsetByName(names[0x94], 0, g_btlMotionOfsY30);
    BtlMotionOffsetTrackKeysByName(names[0x95], 0, g_btlMotionOfsY30);
    break;
  default:
    handled = false;
    break;
  }
  if (handled) {
    g_btlKindSetupMask |= 1u << (self->base.base.unk08 & 0x1f);
  }
}
