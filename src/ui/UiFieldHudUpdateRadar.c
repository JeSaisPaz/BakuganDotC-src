// bdc 0x088d0e14 UiFieldHudUpdateRadar
#include "bdc.h"

/* Radar/minimap step of the field HUD screen (task 3001, `UiFieldHudCtor`; sprite array
   `base.data`): computes the view angle from `viewDir` (`atan2f`), then walks the actor list
   (`setupState`) and, from slot 0x48, gives each non-player actor a blip: template 16 for NPC
   models 0x4e..0x53, else template 15, shown and stencil-tested against 8 (models 7/0x1f hidden
   on stage 1, model 0x55 hidden), placed by `UiFieldHudPlaceRadarBlip`. NPC models also get a
   view cone (template 25) scaled by `viewDist * 0.018`, sized, moved onto the blip and turned by
   `viewHeading + viewAngle - pi/2`; while the field guard-blind state is 2 the cone is shown
   (cloaked guards 0x4f: blip and cone follow `ActorNpcCloakIsHidden`), otherwise the cone is
   hidden; a cone whose view-cone object is invisible is hidden. Leftover actor slots up to
   `iconBase + 0x48` are hidden. Then the target gimmicks (`targetMark` list): cameras 0xbd9 get
   blip 16 plus cone 25 turned by `viewAngle - (rot.y + sweepAngle)` (shown only in guard-blind
   state 2), 0xbc7 gets blip 63, 0x1778/0xbdf get blip 15, shown and placed only while `active`
   and vtable slot 17 returns 0. Slots up to `iconBase + targetCount + 0x48` are hidden, and
   sprite 14 is placed at the field's player start. */

#define HUD_SPRITE(i) (((GfxSprite **)self->base.data)[i])

/* Rotates every matrix row (x, y, z, w) about Z: (c*x - s*y, s*x + c*y, z, w) (VFPU `vrot` +
   `vmmul.q E200, E100, E000`); `vrot` takes quarter turns, so the angle is scaled by S703 = 2/pi. */
static inline void UiFieldHudRotateMatrix(float *matrix, float angle)
{
  float t;
  float c;
  float s;
  float x;
  float y;
  s32 i;

  t = angle * 0.636619747f;
  c = VfCosQuarter(t);
  s = VfSinQuarter(t);
  for (i = 0; i < 4; i++) {
    x = matrix[i * 4 + 0];
    y = matrix[i * 4 + 1];
    matrix[i * 4 + 0] = c * x + -s * y;
    matrix[i * 4 + 1] = s * x + c * y;
  }
}

void UiFieldHudUpdateRadar(UiFieldHud *self)

{
  float viewAngle;
  float angle;
  float coneScale;
  float w;
  GfxSprite *spr;
  Actor *actor;
  ActorNpc *npc;
  GameGimmick *g;
  const VtblEntry *vt;
  s32 slot;

  viewAngle = atan2f(self->viewDir[0], self->viewDir[1]) - 1.5707964f;
  slot = 0x48;
  for (actor = *(Actor **)self->setupState; actor != NULL;
       actor = (Actor *)actor->base.base.next) {
    if (actor->isPlayer != 0) {
      continue;
    }
    if (actor->base.base.unk08 < 0x4e || actor->base.base.unk08 >= 0x54) {
      GfxSpriteCopy(HUD_SPRITE(15), HUD_SPRITE(slot));
    } else {
      GfxSpriteCopy(HUD_SPRITE(16), HUD_SPRITE(slot));
    }
    HUD_SPRITE(slot)->flags |= 1;
    GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
    if (g_scriptGlobalVars[1] == 1 &&
        (actor->base.base.unk08 == 0x1f || actor->base.base.unk08 == 7)) {
      HUD_SPRITE(slot)->flags &= ~1u;
    }
    if (actor->base.base.unk08 == 0x55) {
      HUD_SPRITE(slot)->flags &= ~1u;
    }
    UiFieldHudPlaceRadarBlip(self, actor->base.pos, slot);
    if (actor->base.base.unk08 >= 0x4e && actor->base.base.unk08 < 0x54) {
      npc = (ActorNpc *)actor;
      slot++;
      GfxSpriteCopy(HUD_SPRITE(25), HUD_SPRITE(slot));
      coneScale = npc->viewDist * 0.018f;
      GfxSpriteSetScaleRotation(HUD_SPRITE(slot), coneScale, coneScale,
                                npc->viewHeading + 1.5707964f, false);
      w = GfxSpriteGetWidth(HUD_SPRITE(slot)) * coneScale;
      spr = HUD_SPRITE(slot);
      UiSpriteSetSize(w, GfxSpriteGetHeight(spr) * coneScale, spr);
      HUD_SPRITE(slot)->posX = HUD_SPRITE(slot - 1)->posX;
      HUD_SPRITE(slot)->posY = HUD_SPRITE(slot - 1)->posY;
      GfxSpriteResetMatrix(HUD_SPRITE(slot));
      UiFieldHudRotateMatrix(HUD_SPRITE(slot)->matrix, npc->viewHeading + viewAngle);
      if (((GameFieldTask *)GameFieldFindTask())->guardBlind.state == 2) {
        if (actor->base.base.unk08 == 0x4f) {
          if (ActorNpcCloakIsHidden((ActorNpcCloak *)actor)) {
            HUD_SPRITE(slot - 1)->flags &= ~1u;
            HUD_SPRITE(slot)->flags &= ~1u;
          } else {
            HUD_SPRITE(slot - 1)->flags |= 1;
            HUD_SPRITE(slot)->flags |= 1;
          }
        } else {
          HUD_SPRITE(slot)->flags |= 1;
        }
        GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      } else {
        if (actor->base.base.unk08 == 0x4f) {
          if (ActorNpcCloakIsHidden((ActorNpcCloak *)actor)) {
            HUD_SPRITE(slot - 1)->flags &= ~1u;
          } else {
            HUD_SPRITE(slot - 1)->flags |= 1;
          }
        }
        HUD_SPRITE(slot)->flags &= ~1u;
      }
      if (((ActorNpcViewCone *)npc->viewCone)->visible == 0) {
        HUD_SPRITE(slot)->flags &= ~1u;
      }
    }
    slot++;
  }
  while (slot < *(s32 *)self->iconBase + 0x48) {
    HUD_SPRITE(slot)->flags &= ~1u;
    slot++;
  }

  angle = atan2f(self->viewDir[0], self->viewDir[1]);
  for (g = *(GameGimmick **)self->targetMark; g != NULL; g = (GameGimmick *)g->base.base.next) {
    if (g->typeId == 0xbd9) {
      GfxSpriteCopy(HUD_SPRITE(16), HUD_SPRITE(slot));
      HUD_SPRITE(slot)->flags |= 1;
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      UiFieldHudPlaceRadarBlip(self, g->base.pos, slot);
      slot++;
      GfxSpriteCopy(HUD_SPRITE(25), HUD_SPRITE(slot));
      HUD_SPRITE(slot)->flags |= 1;
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      HUD_SPRITE(slot)->posX = HUD_SPRITE(slot - 1)->posX;
      HUD_SPRITE(slot)->posY = HUD_SPRITE(slot - 1)->posY;
      GfxSpriteResetMatrix(HUD_SPRITE(slot));
      UiFieldHudRotateMatrix(HUD_SPRITE(slot)->matrix,
                             angle - (g->base.rot[1] + ((GameGimmickCamera *)g)->sweepAngle));
      if (((GameFieldTask *)GameFieldFindTask())->guardBlind.state == 2) {
        HUD_SPRITE(slot)->flags |= 1;
      } else {
        HUD_SPRITE(slot)->flags &= ~1u;
      }
      slot++;
    } else if (g->typeId == 0xbc7) {
      GfxSpriteCopy(HUD_SPRITE(63), HUD_SPRITE(slot));
      HUD_SPRITE(slot)->flags |= 1;
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      UiFieldHudPlaceRadarBlip(self, g->base.pos, slot);
      slot++;
    } else if (g->typeId == 0x1778 || g->typeId == 0xbdf) {
      GfxSpriteCopy(HUD_SPRITE(15), HUD_SPRITE(slot));
      HUD_SPRITE(slot)->flags |= 1;
      GfxSpriteSetStencilTest(HUD_SPRITE(slot), true, 8);
      if (g->active != 0) {
        vt = &((const VtblEntry *)g->base.base.vtable)[17];
        if (((s32 (*)(void *))vt->fn)((u8 *)g + vt->delta) != 0) {
          HUD_SPRITE(slot)->flags &= ~1u;
        } else {
          HUD_SPRITE(slot)->flags |= 1;
          UiFieldHudPlaceRadarBlip(self, g->base.pos, slot);
        }
      } else {
        HUD_SPRITE(slot)->flags &= ~1u;
      }
      slot++;
    }
  }
  while (slot < *(s32 *)self->iconBase + self->targetCount + 0x48) {
    HUD_SPRITE(slot)->flags &= ~1u;
    slot++;
  }
  UiFieldHudPlaceRadarBlip(self, ((GameFieldTask *)GameFieldFindTask())->playerStart, 14);
}
