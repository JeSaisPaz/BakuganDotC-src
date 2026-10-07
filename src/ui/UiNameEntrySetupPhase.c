// bdc 0x08805f30 UiNameEntrySetupPhase
#include "bdc.h"

/* Phase 1 of the name entry screen (`UiNameEntry`): allocates the 43-entry sprite
   table (`base.data`) and fills entries 0..0x37 from layout 0x38 (`UiLayoutCreateSprites`);
   creates the key and name text printers (font 3, scale 0.9 / 0.7, wrap width 32 / 20); clones
   sprites 26, 10 and 2 into 34, 35 and 36 and sprites 4-9 into black drop shadows 37-42 one unit
   behind and offset by (1, 1); tints and positions the keyboard/name panels; hides and makes
   transparent sprites 1-42; creates the avatar (`g_btlModelNames[47]`, `"12_Edit_man.gmo"`, fog off,
   turned and moved to x 480) and the pedestal `"menu_daiza.gmo"` (specular 0.6 grey, power 8, fog
   off), points `g_gfxFogParams` at `g_nameEntryFog`, rebuilds the pedestal node `"_02_base"`
   matrix as a -pi/2 rotation about X keeping its translation row, plays avatar motion 1 (looping),
   starts BGM track 0x16 (looping) and switches to phase 2. A failed printer or model allocation
   leaves the field NULL and is used anyway. */

void UiNameEntrySetupPhase(UiNameEntry *self)
{
  GfxSprite **sprites;
  GfxSprite *s;
  UiTextPrinter *printer;
  GfxModel *model;
  GmoNode *node;
  bool fromLow;
  float w;
  float z;
  int i;
  float uvRect[4] __attribute__((aligned(16)));
  float specular[4] __attribute__((aligned(16)));
  float savedRow[4];
  float *m;
  float angle;
  float c;
  float sn;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  sprites = MemAlloc(43 * sizeof(GfxSprite *), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  self->base.data = sprites;
  UiLayoutCreateSprites(self->base.spriteLayer, sprites, 0x38);

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  printer = MemAlloc(sizeof(UiTextPrinter), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (printer != NULL) {
    UiTextPrinterCtor(printer, NULL, NULL);
  }
  self->keyText = printer;
  UiTextPrinterSetFont(printer, 3);
  ((UiTextPrinter *)self->keyText)->scale = 0.9f;
  ((UiTextPrinter *)self->keyText)->wrapWidth = 32.0f;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  printer = MemAlloc(sizeof(UiTextPrinter), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (printer != NULL) {
    UiTextPrinterCtor(printer, NULL, NULL);
  }
  self->nameText = printer;
  UiTextPrinterSetFont(printer, 3);
  ((UiTextPrinter *)self->nameText)->scale = 0.7f;
  ((UiTextPrinter *)self->nameText)->wrapWidth = 20.0f;

  GfxSpriteCenterPivot(sprites[26]);
  GfxSpriteSetScaleRotation(sprites[26], 1.0f, 1.0f, 0.0f, false);

  sprites[34] = UiNameEntryCloneSprite(sprites[26], self->base.spriteLayer);
  s = sprites[34];
  w = GfxSpriteGetWidth(sprites[26]);
  UiSpriteSetSize(w, GfxSpriteGetHeight(sprites[26]), s);

  sprites[35] = UiNameEntryCloneSprite(sprites[10], self->base.spriteLayer);
  s = sprites[35];
  w = GfxSpriteGetWidth(sprites[10]);
  UiSpriteSetSize(w, GfxSpriteGetHeight(sprites[10]), s);

  sprites[36] = UiNameEntryCloneSprite(sprites[2], self->base.spriteLayer);
  uvRect[0] = 0.0f;
  uvRect[1] = 8.0f;
  uvRect[2] = 32.0f;
  uvRect[3] = 32.0f;
  GfxSpriteSetUvRectXYWH(sprites[36], uvRect);
  UiSpriteSetSize(32.0f, 32.0f, sprites[36]);

  sprites[19]->posZ = sprites[19]->posZ - 1.0f;
  sprites[20]->posZ = sprites[20]->posZ - 1.0f;

  /* sprites 37..42: black shadows of 4..9, one unit behind and offset by (1, 1) */
  for (i = 0; i < 6; i++) {
    sprites[4 + i]->posZ = sprites[4 + i]->posZ - 1.0f;
    sprites[37 + i] = UiNameEntryCloneSprite(sprites[4 + i], self->base.spriteLayer);
    s = sprites[37 + i];
    /* lv.q/sv.q 16-byte copy: tint[0..2] and alpha */
    s->tint[0] = g_colorBlack.x;
    s->tint[1] = g_colorBlack.y;
    s->tint[2] = g_colorBlack.z;
    s->alpha = g_colorBlack.w;
    sprites[37 + i]->posZ = sprites[37 + i]->posZ + 1.0f;
    sprites[37 + i]->posX = sprites[37 + i]->posX + 1.0f;
    sprites[37 + i]->posY = sprites[37 + i]->posY + 1.0f;
  }

  sprites[0]->layerMask = 2;
  sprites[11]->layerMask = 4;
  sprites[19]->layerMask = 4;
  sprites[20]->layerMask = 4;
  sprites[25]->layerMask = 4;

  /* chained 16-byte tint/alpha copies: 22 <- dark green, 15 <- 22, 14 <- 15, 2 <- 14 */
  s = sprites[22];
  s->tint[0] = g_nameEntryColorDarkGreen.x;
  s->tint[1] = g_nameEntryColorDarkGreen.y;
  s->tint[2] = g_nameEntryColorDarkGreen.z;
  s->alpha = g_nameEntryColorDarkGreen.w;
  sprites[15]->tint[0] = sprites[22]->tint[0];
  sprites[15]->tint[1] = sprites[22]->tint[1];
  sprites[15]->tint[2] = sprites[22]->tint[2];
  sprites[15]->alpha = sprites[22]->alpha;
  sprites[14]->tint[0] = sprites[15]->tint[0];
  sprites[14]->tint[1] = sprites[15]->tint[1];
  sprites[14]->tint[2] = sprites[15]->tint[2];
  sprites[14]->alpha = sprites[15]->alpha;
  sprites[2]->tint[0] = sprites[14]->tint[0];
  sprites[2]->tint[1] = sprites[14]->tint[1];
  sprites[2]->tint[2] = sprites[14]->tint[2];
  sprites[2]->alpha = sprites[14]->alpha;

  /* 24 <- dim green, 23 <- 24 */
  s = sprites[24];
  s->tint[0] = g_nameEntryColorDimGreen.x;
  s->tint[1] = g_nameEntryColorDimGreen.y;
  s->tint[2] = g_nameEntryColorDimGreen.z;
  s->alpha = g_nameEntryColorDimGreen.w;
  sprites[23]->tint[0] = s->tint[0];
  sprites[23]->tint[1] = s->tint[1];
  sprites[23]->tint[2] = s->tint[2];
  sprites[23]->alpha = s->alpha;

  s = sprites[3];
  s->tint[0] = g_nameEntryColorGreen.x;
  s->tint[1] = g_nameEntryColorGreen.y;
  s->tint[2] = g_nameEntryColorGreen.z;
  s->alpha = g_nameEntryColorGreen.w;
  s = sprites[4];
  s->tint[0] = g_nameEntryColorLightGreen.x;
  s->tint[1] = g_nameEntryColorLightGreen.y;
  s->tint[2] = g_nameEntryColorLightGreen.z;
  s->alpha = g_nameEntryColorLightGreen.w;
  s = sprites[36];
  s->tint[0] = g_nameEntryColorDimGreen.x;
  s->tint[1] = g_nameEntryColorDimGreen.y;
  s->tint[2] = g_nameEntryColorDimGreen.z;
  s->alpha = g_nameEntryColorDimGreen.w;

  sprites[13]->alpha = 0.5f;
  sprites[13]->textureSlot = 1;
  sprites[12]->alpha = 0.5f;

  sprites[25]->posX = 152.5f;
  sprites[25]->posY = 33.5f;
  sprites[25]->posZ = sprites[18]->posZ - 1.0f;

  z = sprites[24]->posZ;
  sprites[23]->posZ = z;
  sprites[22]->posZ = z;
  sprites[15]->posZ = z;
  sprites[14]->posZ = z;

  z = sprites[24]->posZ - 1.0f;
  sprites[20]->posZ = z;
  sprites[19]->posZ = z;

  s = sprites[11];
  z = sprites[3]->posZ - 1.0f;
  s->posX = 139.0f;
  s->posY = 63.5f;
  s->posZ = z;
  s->posW = 0.0f;

  sprites[19]->posZ = sprites[19]->posZ - 1.0f;
  sprites[20]->posZ = sprites[20]->posZ - 1.0f;

  GfxSpriteCenterPivot(sprites[10]);
  GfxSpriteResetMatrix(sprites[10]);
  GfxSpriteCenterPivot(sprites[16]);
  GfxSpriteResetMatrix(sprites[16]);
  GfxSpriteCenterPivot(sprites[17]);
  GfxSpriteResetMatrix(sprites[17]);
  GfxSpriteSetVCell(2.0f, sprites[27]);
  GfxSpriteSetVCell(1.0f, sprites[28]);

  s = sprites[36];
  z = sprites[2]->posZ - 0.5f;
  s->posX = 93.0f;
  s->posY = 142.0f;
  s->posZ = z;
  s->posW = 0.0f;

  for (i = 1; i < 43; i++) {
    sprites[i]->alpha = 0.0f;
    sprites[i]->flags &= ~1u;
  }

  /* avatar */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  model = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (model != NULL) {
    GfxModelCtor(model, g_btlModelNames[47], 0);
  }
  self->avatar = model;
  model->fogEnabled = 0;
  g_gfxFogParams = &g_nameEntryFog;
  ((GfxModel *)self->avatar)->rot[0] = ((GfxModel *)self->avatar)->rot[0] + 1.5707964f;
  ((GfxModel *)self->avatar)->rot[1] = ((GfxModel *)self->avatar)->rot[1] + 2.8274333f;
  ((GfxModel *)self->avatar)->rot[2] = ((GfxModel *)self->avatar)->rot[2] + 1.5707964f;
  ((GfxModel *)self->avatar)->pos[0] = 480.0f;

  /* pedestal */
  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  model = MemAlloc(sizeof(GfxModel), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  if (model != NULL) {
    GfxModelCtor(model, "menu_daiza.gmo", 0);
  }
  self->baseModel = model;
  specular[0] = 0.6f;
  specular[1] = 0.6f;
  specular[2] = 0.6f;
  specular[3] = 1.0f;
  GfxModelSetSpecular(8.0f, model, specular, NULL);
  ((GfxModel *)self->baseModel)->fogEnabled = 0;
  g_gfxFogParams = &g_nameEntryFog;

  node = (GmoNode *)GfxModelFindNode((GfxModel *)self->baseModel, "_02_base");
  self->baseNode = node;
  self->baseMatrix = node->localMatrix;
  /* baseMatrix = rotX(-pi/2): rows I, [0 C S 0], [0 -S C 0], I (vrot of -pi/2 times the bank's 2/pi),
     then the saved translation row (+0x30) is put back. */
  m = self->baseMatrix;
  for (i = 0; i < 4; i++) {
    savedRow[i] = m[12 + i];
  }
  angle = -1.5707964f;
  c = __builtin_cosf(angle);
  sn = __builtin_sinf(angle);
  m[0] = 1.0f;
  m[1] = 0.0f;
  m[2] = 0.0f;
  m[3] = 0.0f;
  m[4] = 0.0f;
  m[5] = c;
  m[6] = sn;
  m[7] = 0.0f;
  m[8] = 0.0f;
  m[9] = -sn;
  m[10] = c;
  m[11] = 0.0f;
  m[12] = 0.0f;
  m[13] = 0.0f;
  m[14] = 0.0f;
  m[15] = 1.0f;
  m = self->baseMatrix;
  for (i = 0; i < 4; i++) {
    m[12 + i] = savedRow[i];
  }

  UiNameEntryPlayAvatarMotion(self, 1, 1);
  SndBgmPlayerPlayTrack(SndBgmPlayerGet(0), 0x16, 1, 0);
  self->base.phase = 2;
}
