// bdc 0x08912804 UiUpgradeCtor
#include "bdc.h"

/* Constructor of the Bakugan upgrade screen, task id 490 (0x1ea) (base `UiScreenCtor`, vtable
   `g_uiUpgradeVtbl`). Object size 0x16c0. Allocates (low heap) the 4-byte `bgData` block and the
   103-entry sprite table `data` (0x19c bytes), sets frame mode 0 and stick-as-dpad, resets the
   model/camera, text colour (white) and menu state, takes the selected Bakugan from the save
   profile (`curBakugan`), puts the active fader at sort key 20000, creates the message text
   printer (`UiTextPrinterCreate(1)`, wrap 232, scale 0.7, width scale 0.42) and clears the message.
   The screen is the ability/upgrade menu (`"up_grade.fab"` background, `"DMUpgrade"` messages,
   `"up_waku_01/02"`, `"c_set_OK_bo_1/2"` and `"ability03_a"` sprites, Bakugan names
   `"f_cha_name_baku_%02d"`). Returns `self`. */

UiUpgrade *UiUpgradeCtor(UiUpgrade *self)
{
  bool wasLow;
  void *block;
  UiTextPrinter *printer;
  s32 bakugan;

  UiScreenCtor((CoreTask *)self);
  self->base.base.vtable = g_uiUpgradeVtbl;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(sizeof(GfxFab *), NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  self->base.bgData = block;
  memset(block, 0, sizeof(GfxFab *));
  self->base.unk58 = 0;
  self->base.bgAnimList = NULL;
  self->base.unk5c = 0;

  MemLock();
  wasLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  block = MemAlloc(103 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(wasLow);
  MemUnlock();
  self->base.data = block;

  UiScreenSetFrameMode((CoreTask *)self, 0);
  self->base.pad->stickEmulatesDpad = 1;
  self->base.phaseStep = 0;
  self->model = NULL;
  self->camera = NULL;
  self->textColor[0] = 1.0f;
  self->textColor[1] = 1.0f;
  self->textColor[2] = 1.0f;
  self->textColor[3] = 1.0f;
  self->unk1690 = 0.0f;
  self->unk1694 = 0;
  self->focus = 0;
  self->unk169c = 0.0f;
  self->confirmChoice = 0;
  self->confirmStep = 0;
  bakugan = SaveGetProfile()->data->curBakugan;
  self->points = 0;
  self->bakugan = bakugan;
  self->unk16b0 = 0;

  GfxGetActiveFader()->sortKey = 20000.0f;
  printer = UiTextPrinterCreate(1);
  self->textPrinter = printer;
  printer->wrapWidth = 232.0f;
  self->textPrinter->scale = 0.7f;
  self->textPrinter->widthScale = 0.42f;
  strcpy(self->message, "");
  return self;
}
