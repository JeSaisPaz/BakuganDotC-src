// bdc 0x089923b8 UiUnlockCodeCtor
#include "bdc.h"

/* Constructor of UiUnlockCode, the special-reward unlock-code screen (`DWSpecialUnlock`: "Enter
   your exclusive 8-digit unlock code", `data/2d/%s/name.lzs`); task id 316 (0x13c), object size
   0x124. Runs `UiScreenCtor`, installs vtable `g_unlockCodeVtbl`, clears its fields, sets frame mode 1
   and pad byte `+0x3c`, then caches the number of rewards already unlocked
   (`UiUnlockCodeSyncRewards` → `+0x118` registeredCount), the code-entry mode (`UiUnlockCodeIsDimensionsMode`
   → `+0x108`) and the digit count (`UiUnlockCodeGetDigitCount` → `+0x104`), and fills the
   digit array `+0xb4` with -1. */

UiUnlockCode *UiUnlockCodeCtor(UiUnlockCode *self)

{
  s32 count;
  s32 i;

  UiScreenCtor((CoreTask *)self);
  (self->base).base.vtable = g_unlockCodeVtbl;
  self->package = (void *)0x0;
  self->commonPackage = (void *)0x0;
  self->removeRequested = '\0';
  self->caret = 0;
  self->keyColumn = 0;
  self->keyRow = 0;
  self->pageOffset = 0;
  self->inputFlag = '\0';
  self->page = '\0';
  self->introTimer = 0;
  self->keyText = (UiTextPrinter *)0x0;
  self->digitText = (UiTextPrinter *)0x0;
  self->lastInput = -1;
  self->inputCounter = 0;
  self->holdTimer = 0;
  self->cursorAngle = 0;
  self->keyPopOn = '\0';
  self->keyPopTimer = 0;
  self->okFlashT = 0.0f;
  self->dimensionsMode = 0;
  UiScreenSetFrameMode((CoreTask *)self,1);
  ((self->base).pad)->stickEmulatesDpad = '\x01';
  self->registeredCount = UiUnlockCodeSyncRewards(self);
  self->dimensionsMode = UiUnlockCodeIsDimensionsMode();
  count = UiUnlockCodeGetDigitCount(self);
  self->digitCount = count;
  for (i = 0; i < count; i++) {
    self->entered[i] = -1;
  }
  return self;
}

