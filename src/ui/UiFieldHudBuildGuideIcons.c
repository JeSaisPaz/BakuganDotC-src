// bdc 0x088cf25c UiFieldHudBuildGuideIcons
#include "bdc.h"

/* Builds the radar sprites of the field HUD (task 3001, `UiFieldHudCtor`; sprite array
   `base.data`, filled with the 12 layout sprites by `UiFieldHudSetupPhase`): reads the story
   chapter (`GameFieldGetStoryHint`, also `hintId`; chapter -1 sets phase 5) and copies its row of
   `g_uiFieldHudMapRects` into `mapRect`; creates the radar mask sprite 0x43 (copy of sprite 0,
   stencil write 8), sizes the radar map sprite 1 from its texture scaled by
   `mapRect[0] / g_uiFieldHudMapRects[2][0] * 0.7` (also `radarSize`, `radarHalf`), creates sprite
   0x44 (copy of 0x15) and the hidden guide icons 0x45..0x47 (`guide_ico_13`, `guide_ico_12`,
   `guide_ico_05`, `UiFieldHudCreateHiddenSprite`) placed over sprites 0x26/0x27/0x26, and sets up
   the blip templates 15, 16 and 25 (view cone; both orange-red `g_colorOrangeRed`), 63 and 14. Then, from slot 0x48, one blip per
   non-player actor of the actor list (`setupState`): template 15, or for NPC models 0x4e..0x53
   template 16 plus a view cone (template 25 scaled by `viewDist * 0.018`, rotated by
   `viewHeading + pi/2`); one or two per target gimmick (`targetMark` list: 0xbd9 → 16 + cone 25
   scaled 1.8, 0xbc7 → 63, 0x1778/0xbdf → 15); then template-15 blips up to slot
   `iconBase + targetCount + 0x48`. Every new sprite is allocated as a `GfxSprite`, added to the
   layer `base.spriteLayer`, stencil-tested against 8 and hidden; finally sprites 0..0x47 are
   hidden too. */

#define HUD_SPRITE(i) (((GfxSprite **)self->base.data)[i])

/* Inlined `new GfxSprite`: allocate 0x160 bytes from the low end of the heap and construct. */
#define HUD_NEW_SPRITE(dst)                                                                       \
  do {                                                                                            \
    bool fromLow_;                                                                                \
    GfxSprite *mem_;                                                                              \
    GfxSprite *sprite_ = NULL;                                                                    \
    MemLock();                                                                                    \
    fromLow_ = MemIsAllocFromLow();                                                               \
    MemSetAllocFromLow(true);                                                                     \
    mem_ = (GfxSprite *)MemAlloc(sizeof(GfxSprite), NULL, 0);                                                 \
    MemSetAllocFromLow(fromLow_);                                                                 \
    MemUnlock();                                                                                  \
    if (mem_ != NULL) {                                                                           \
      GfxSpriteCtor(mem_);                                                                        \
      sprite_ = mem_;                                                                             \
    }                                                                                             \
    (dst) = sprite_;                                                                              \
  } while (0)

void UiFieldHudBuildGuideIcons(UiFieldHud *self)

{
  s32 chapter;
  float uv[4];
  float *rect;
  float scale;
  float coneScale;
  float w;
  float h;
  GfxSprite *spr;
  Actor *actor;
  ActorNpc *npc;
  GameGimmick *g;
  s32 slot;
  s32 i;

  chapter = -1;
  self->hintId = 0;
  GameFieldGetStoryHint(GameFieldFindTask(), &chapter, &self->hintId);
  if (chapter == -1) {
    self->base.phase = 5;
  }
  rect = g_uiFieldHudMapRects[chapter];
  self->mapRect[0] = rect[0];
  self->mapRect[1] = rect[1];
  self->mapRect[2] = rect[2];
  self->mapRect[3] = rect[3];

  /* Radar mask (sprite 0x43): copy of the radar frame sprite 0. */
  HUD_NEW_SPRITE(HUD_SPRITE(0x43));
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(0x43));
  GfxSpriteCopy(HUD_SPRITE(0), HUD_SPRITE(0x43));
  spr = HUD_SPRITE(0);
  w = GfxSpriteGetWidth(spr) - 1.0f;
  h = GfxSpriteGetHeight(HUD_SPRITE(0)) - 1.0f;
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = w;
  uv[3] = h;
  GfxSpriteSetUvRectXYWH(spr, uv);
  GfxSpriteSetScaleRotation(HUD_SPRITE(0), 1.5f, 1.5f, 0.0f, false);
  w = GfxSpriteGetWidth(HUD_SPRITE(0)) * 1.5f;
  spr = HUD_SPRITE(0);
  UiSpriteSetSize(w, GfxSpriteGetHeight(spr) * 1.5f, spr);
  HUD_SPRITE(0)->flags &= ~0x20u;

  /* Radar map (sprite 1) sized from its texture. */
  scale = self->mapRect[0] / g_uiFieldHudMapRects[2][0] * 0.7f;
  self->radarSize[0] = (float)GfxTextureGetWidth(HUD_SPRITE(1)->texture) * scale;
  self->radarSize[1] = (float)GfxTextureGetHeight(HUD_SPRITE(1)->texture) * scale;
  spr = HUD_SPRITE(1);
  w = (float)GfxTextureGetWidth(spr->texture);
  h = (float)GfxTextureGetHeight(HUD_SPRITE(1)->texture);
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = w;
  uv[3] = h;
  GfxSpriteSetUvRectXYWH(spr, uv);
  spr = HUD_SPRITE(1);
  w = (float)GfxTextureGetWidth(spr->texture) * scale;
  UiSpriteSetSize(w, (float)GfxTextureGetHeight(HUD_SPRITE(1)->texture) * scale, spr);
  GfxSpriteCenterPivot(HUD_SPRITE(1));
  GfxSpriteResetMatrix(HUD_SPRITE(1));
  GfxSpriteSetStencilTest(HUD_SPRITE(1), true, 8);
  HUD_SPRITE(1)->flags &= ~0x20u;
  self->radarHalf[0] = GfxSpriteGetWidth(HUD_SPRITE(1)) * 0.5f;
  self->radarHalf[1] = GfxSpriteGetHeight(HUD_SPRITE(1)) * 0.5f;

  spr = HUD_SPRITE(0x43);
  w = GfxSpriteGetWidth(spr) - 20.0f;
  h = GfxSpriteGetHeight(HUD_SPRITE(0x43)) - 20.0f;
  uv[2] = w;
  uv[0] = 11.0f;
  uv[1] = 11.0f;
  uv[3] = h;
  GfxSpriteSetUvRectXYWH(spr, uv);
  GfxSpriteSetScaleRotation(HUD_SPRITE(0x43), scale, scale, 0.0f, false);
  spr = HUD_SPRITE(0x43);
  w = GfxSpriteGetWidth(HUD_SPRITE(0)) - 29.0f;
  UiSpriteSetSize(w, GfxSpriteGetHeight(HUD_SPRITE(0)) - 29.0f, spr);
  GfxSpriteSetStencilWrite(HUD_SPRITE(0x43), true, 8);

  /* Sprite 0x44: copy of 0x15. */
  HUD_NEW_SPRITE(HUD_SPRITE(0x44));
  GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(0x44));
  GfxSpriteCopy(HUD_SPRITE(0x15), HUD_SPRITE(0x44));

  /* Hidden guide icons 0x45..0x47 over sprites 0x26, 0x27, 0x26. */
  HUD_SPRITE(0x45) = UiFieldHudCreateHiddenSprite("guide_ico_13", self->base.spriteLayer);
  spr = HUD_SPRITE(0x45);
  w = GfxSpriteGetWidth(HUD_SPRITE(0x26));
  UiSpriteSetSize(w, GfxSpriteGetHeight(HUD_SPRITE(0x26)), spr);
  spr = HUD_SPRITE(0x45);
  w = GfxSpriteGetWidth(HUD_SPRITE(0x26));
  h = GfxSpriteGetHeight(HUD_SPRITE(0x26));
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = w;
  uv[3] = h;
  GfxSpriteSetUvRectXYWH(spr, uv);
  /* position copy (lv.q/sv.q quad) */
  HUD_SPRITE(0x45)->posX = HUD_SPRITE(0x26)->posX;
  HUD_SPRITE(0x45)->posY = HUD_SPRITE(0x26)->posY;
  HUD_SPRITE(0x45)->posZ = HUD_SPRITE(0x26)->posZ;
  HUD_SPRITE(0x45)->posW = HUD_SPRITE(0x26)->posW;

  HUD_SPRITE(0x46) = UiFieldHudCreateHiddenSprite("guide_ico_12", self->base.spriteLayer);
  spr = HUD_SPRITE(0x46);
  w = GfxSpriteGetWidth(HUD_SPRITE(0x27));
  UiSpriteSetSize(w, GfxSpriteGetHeight(HUD_SPRITE(0x27)), spr);
  spr = HUD_SPRITE(0x46);
  w = GfxSpriteGetWidth(HUD_SPRITE(0x27));
  h = GfxSpriteGetHeight(HUD_SPRITE(0x27));
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = w;
  uv[3] = h;
  GfxSpriteSetUvRectXYWH(spr, uv);
  HUD_SPRITE(0x46)->posX = HUD_SPRITE(0x27)->posX;
  HUD_SPRITE(0x46)->posY = HUD_SPRITE(0x27)->posY;
  HUD_SPRITE(0x46)->posZ = HUD_SPRITE(0x27)->posZ;
  HUD_SPRITE(0x46)->posW = HUD_SPRITE(0x27)->posW;

  HUD_SPRITE(0x47) = UiFieldHudCreateHiddenSprite("guide_ico_05", self->base.spriteLayer);
  spr = HUD_SPRITE(0x47);
  w = GfxSpriteGetWidth(HUD_SPRITE(0x26));
  UiSpriteSetSize(w, GfxSpriteGetHeight(HUD_SPRITE(0x26)), spr);
  spr = HUD_SPRITE(0x47);
  w = GfxSpriteGetWidth(HUD_SPRITE(0x26));
  h = GfxSpriteGetHeight(HUD_SPRITE(0x26));
  uv[0] = 0.0f;
  uv[1] = 0.0f;
  uv[2] = w;
  uv[3] = h;
  GfxSpriteSetUvRectXYWH(spr, uv);
  HUD_SPRITE(0x47)->posX = HUD_SPRITE(0x26)->posX;
  HUD_SPRITE(0x47)->posY = HUD_SPRITE(0x26)->posY;
  HUD_SPRITE(0x47)->posZ = HUD_SPRITE(0x26)->posZ;
  HUD_SPRITE(0x47)->posW = HUD_SPRITE(0x26)->posW;

  GfxSpriteCenterPivot(HUD_SPRITE(13));
  GfxSpriteResetMatrix(HUD_SPRITE(13));
  GfxSpriteSetStencilTest(HUD_SPRITE(13), true, 8);
  HUD_SPRITE(13)->flags |= 0x20;
  GfxSpriteSetScaleRotation(HUD_SPRITE(0x15), 1.4f, 1.4f, 0.0f, false);
  HUD_SPRITE(0)->flags |= 0x20;
  GfxSpriteSetScaleRotation(HUD_SPRITE(0x44), 1.4f, 1.4f, 0.0f, false);
  HUD_SPRITE(0x44)->flags |= 0x20;
  HUD_SPRITE(0x44)->posZ = HUD_SPRITE(0x15)->posZ;
  HUD_SPRITE(0x2d)->addColor[1] = 1.0f;
  HUD_SPRITE(0x2e)->addColor[1] = 1.0f;

  /* Blip templates: 16 and 25 (view cone) red, recoloured orange-red below; 15 with red cleared. */
  spr = HUD_SPRITE(16);
  spr->tint[0] = 1.0f;
  spr->tint[1] = 0.0f;
  spr->tint[2] = 0.0f;
  spr->alpha = 1.0f;
  HUD_SPRITE(16)->posZ -= 5.0f;
  GfxSpriteCenterPivot(HUD_SPRITE(16));
  GfxSpriteResetMatrix(HUD_SPRITE(16));
  spr = HUD_SPRITE(25);
  spr->tint[0] = 1.0f;
  spr->tint[1] = 0.0f;
  spr->tint[2] = 0.0f;
  spr->alpha = 1.0f;
  HUD_SPRITE(25)->posZ -= HUD_SPRITE(16)->posZ + 1.0f;
  GfxSpriteSetBottomCentrePivot(HUD_SPRITE(25));
  GfxSpriteResetMatrix(HUD_SPRITE(25));
  HUD_SPRITE(15)->tint[0] = 0.0f;
  GfxSpriteCenterPivot(HUD_SPRITE(15));
  GfxSpriteResetMatrix(HUD_SPRITE(15));

  /* One blip (plus a view cone for NPC models 0x4e..0x53) per non-player actor. */
  slot = 0x48;
  for (actor = *(Actor **)self->setupState; actor != NULL;
       actor = (Actor *)actor->base.base.next) {
    if (actor->isPlayer != 0) {
      continue;
    }
    HUD_NEW_SPRITE(HUD_SPRITE(slot));
    GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(slot));
    if (actor->base.base.unk08 < 0x4e || actor->base.base.unk08 >= 0x54) {
      GfxSpriteCopy(HUD_SPRITE(15), HUD_SPRITE(slot));
      HUD_SPRITE(slot)->flags &= ~1u;
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
    } else {
      npc = (ActorNpc *)actor;
      GfxSpriteCopy(HUD_SPRITE(16), HUD_SPRITE(slot));
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      HUD_SPRITE(slot)->flags &= ~1u;
      slot++;
      HUD_NEW_SPRITE(HUD_SPRITE(slot));
      GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(slot));
      GfxSpriteCopy(HUD_SPRITE(25), HUD_SPRITE(slot));
      coneScale = npc->viewDist * 0.018f;
      GfxSpriteSetScaleRotation(HUD_SPRITE(slot), coneScale, coneScale,
                                npc->viewHeading + 1.5707964f, false);
      w = GfxSpriteGetWidth(HUD_SPRITE(slot)) * coneScale;
      spr = HUD_SPRITE(slot);
      UiSpriteSetSize(w, GfxSpriteGetHeight(spr) * coneScale, spr);
      HUD_SPRITE(slot)->flags &= ~1u;
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
    }
    if (actor->base.base.unk08 == 0x55) {
      HUD_SPRITE(slot)->flags &= ~1u;
    }
    slot++;
  }

  /* tint + alpha of templates 16 and 25 = g_colorOrangeRed (lv.q/sv.q quad copies) */
  spr = HUD_SPRITE(16);
  spr->tint[0] = g_colorOrangeRed.x;
  spr->tint[1] = g_colorOrangeRed.y;
  spr->tint[2] = g_colorOrangeRed.z;
  spr->alpha = g_colorOrangeRed.w;
  spr = HUD_SPRITE(25);
  spr->tint[0] = g_colorOrangeRed.x;
  spr->tint[1] = g_colorOrangeRed.y;
  spr->tint[2] = g_colorOrangeRed.z;
  spr->alpha = g_colorOrangeRed.w;
  HUD_SPRITE(63)->posZ = HUD_SPRITE(16)->posZ - 1.0f;
  GfxSpriteCenterPivot(HUD_SPRITE(63));
  GfxSpriteResetMatrix(HUD_SPRITE(63));
  HUD_SPRITE(14)->posZ = HUD_SPRITE(63)->posZ - 1.0f;
  GfxSpriteCenterPivot(HUD_SPRITE(14));
  GfxSpriteResetMatrix(HUD_SPRITE(14));
  GfxSpriteSetStencilTest(HUD_SPRITE(14), true, 8);

  /* Target gimmick blips. */
  for (g = *(GameGimmick **)self->targetMark; g != NULL; g = (GameGimmick *)g->base.base.next) {
    if (g->typeId == 0xbd9) {
      HUD_NEW_SPRITE(HUD_SPRITE(slot));
      GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(slot));
      GfxSpriteCopy(HUD_SPRITE(16), HUD_SPRITE(slot));
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      HUD_SPRITE(slot)->flags &= ~1u;
      slot++;
      HUD_NEW_SPRITE(HUD_SPRITE(slot));
      GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(slot));
      GfxSpriteCopy(HUD_SPRITE(25), HUD_SPRITE(slot));
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      HUD_SPRITE(slot)->flags &= ~1u;
      GfxSpriteSetScaleRotation(HUD_SPRITE(slot), 1.8f, 1.8f, 0.0f, false);
      w = GfxSpriteGetWidth(HUD_SPRITE(slot)) * 1.8f;
      spr = HUD_SPRITE(slot);
      UiSpriteSetSize(w, GfxSpriteGetHeight(spr) * 1.8f, spr);
      slot++;
    } else if (g->typeId == 0xbc7) {
      HUD_NEW_SPRITE(HUD_SPRITE(slot));
      GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(slot));
      GfxSpriteCopy(HUD_SPRITE(63), HUD_SPRITE(slot));
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      HUD_SPRITE(slot)->flags &= ~1u;
      slot++;
    } else if (g->typeId == 0x1778 || g->typeId == 0xbdf) {
      HUD_NEW_SPRITE(HUD_SPRITE(slot));
      GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(slot));
      GfxSpriteCopy(HUD_SPRITE(15), HUD_SPRITE(slot));
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      HUD_SPRITE(slot)->flags &= ~1u;
      slot++;
    }
  }

  /* Remaining slots up to the size allocated by UiFieldHudSetupPhase. */
  while (slot < *(s32 *)self->iconBase + self->targetCount + 0x48) {
    HUD_NEW_SPRITE(HUD_SPRITE(slot));
    GfxSpriteLayerAdd(self->base.spriteLayer, (CoreObject *)HUD_SPRITE(slot));
    GfxSpriteCopy(HUD_SPRITE(15), HUD_SPRITE(slot));
    GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
    HUD_SPRITE(slot)->flags &= ~1u;
    slot++;
  }

  for (i = 0; i < 0x48; i++) {
    HUD_SPRITE(i)->flags &= ~1u;
  }
}
