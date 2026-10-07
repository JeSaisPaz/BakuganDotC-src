// bdc 0x088be378 GameFieldUpdateBackgroundBattle
#include "bdc.h"

/* Ambient background battle of stage 1 (called by `GameFieldUpdateWorld` when script variable 1
   is 1): a 151-frame loop (`g_bgBattleFrame`) over two giant Bakugan actors — at frame 0 kind
   0x1f plays `"10_for_one_shot"` (then `"10_for_stay"` when `g_bgBattleFortressTimer` runs out),
   at frame 9 two shot effects 0x55 spawn at fixed points on effect manager `effectMgr2` and sound
   `0x2c000ca` plays when the player is within 1100 units, and at frame 20 kind 7 plays
   `"04_wil_b_stagger"` (then `"04_wil_defense_c"` when `g_bgBattleWildaTimer` runs out), spawns
   hit effect 0x51 on `effectMgr` and, with the player within 300 units, shakes the camera
   (amplitude scaled by distance, half the stagger length) and plays sound `0x2c000c9`.
   The frame-9 normalise of fortress.pos - wilda.pos into a local is never read afterwards. */

void GameFieldUpdateBackgroundBattle(CoreTask *task)
{
  GameFieldTask *gf = (GameFieldTask *)task;
  GameFieldCamera *cam;
  Actor *actor;
  Actor *fortress;
  Actor *wilda;
  Actor *player;
  float *pos;
  float dist;
  float vol;
  float scale;
  float shotA[4];
  float shotB[4];
  float hitPos[4];
  float dir[4];
  float diff[4];

  if (g_bgBattleFrame == 0) {
    for (actor = *(Actor **)ActorGetList(); actor != NULL; actor = (Actor *)actor->base.base.next) {
      if (actor->base.base.unk08 == 0x1f) {
        actor->base.scale[2] = 2.0f;
        actor->base.scale[1] = 2.0f;
        actor->base.scale[0] = 2.0f;
        ActorSetStateBase(actor, 3, 0);
        GfxModelPlayMotionByName(0.2f, &actor->base, g_bgBattleMotionNames[3], false);
        g_bgBattleFortressTimer = (u32)GfxModelGetMotionRemaining(&actor->base);
        break;
      }
    }
  }

  if (g_bgBattleFrame == 9) {
    fortress = NULL;
    wilda = NULL;
    for (actor = *(Actor **)ActorGetList(); actor != NULL; actor = (Actor *)actor->base.base.next) {
      if (actor->base.base.unk08 == 0x1f) {
        fortress = actor;
      } else if (actor->base.base.unk08 == 7) {
        wilda = actor;
      }
      if (fortress != NULL && wilda != NULL) {
        break;
      }
    }
    if (fortress != NULL) {
      pos = fortress->base.pos;
      if (wilda != NULL && gf->effectMgr2 != NULL) {
        /* dir = normalize(fortress.pos - wilda.pos), saturated to [-1,1], zero for a
           zero-length vector; never read afterwards (lane 3 of the store is stale, left out) */
        dir[0] = pos[0] - wilda->base.pos[0];
        dir[1] = pos[1] - wilda->base.pos[1];
        dir[2] = pos[2] - wilda->base.pos[2];
        scale = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
        scale = (scale == 0.0f) ? 0.0f : VfRsq(scale);
        dir[0] = VfSat1(dir[0] * scale);
        dir[1] = VfSat1(dir[1] * scale);
        dir[2] = VfSat1(dir[2] * scale);
        shotA[0] = 101.5f;
        shotA[1] = 202.5f;
        shotA[2] = -801.2f;
        shotA[3] = 0.0f;
        shotB[0] = 309.5f;
        shotB[1] = 202.2f;
        shotB[2] = -668.5f;
        shotB[3] = 0.0f;
        GfxEffectSpawn(gf->effectMgr2, 0x55, shotB);
        GfxEffectSpawn(gf->effectMgr2, 0x55, shotA);
      }
      player = (Actor *)ActorFindPlayer();
      /* dist = |player.pos - pos| (xyz) */
      diff[0] = player->base.pos[0] - pos[0];
      diff[1] = player->base.pos[1] - pos[1];
      diff[2] = player->base.pos[2] - pos[2];
      dist = __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
      dist = dist - 800.0f;
      if (dist < 0.0f) {
        dist = 0.0f;
      }
      vol = (300.0f - dist) * 0.0033333334f;
      if (!(vol <= 0.0f)) {
        SndEmitterCreateAtPos(SndGetListener(), 0x2c000ca, pos, 0, 1);
      }
    }
  }

  if (g_bgBattleFrame == 20) {
    for (actor = *(Actor **)ActorGetList(); actor != NULL; actor = (Actor *)actor->base.base.next) {
      if (actor->base.base.unk08 == 7) {
        actor->base.scale[2] = 2.0f;
        actor->base.scale[1] = 2.0f;
        actor->base.scale[0] = 2.0f;
        ActorSetStateBase(actor, 3, 0);
        GfxModelPlayMotionByName(0.2f, &actor->base, g_bgBattleMotionNames[1], false);
        g_bgBattleWildaTimer = (u32)GfxModelGetMotionRemaining(&actor->base);
        pos = actor->base.pos;
        hitPos[0] = pos[0];
        hitPos[1] = pos[1];
        hitPos[2] = pos[2];
        hitPos[3] = pos[3];
        hitPos[1] = hitPos[1] + 10.0f;
        GfxEffectSpawn(gf->effectMgr, 0x51, hitPos);
        player = (Actor *)ActorFindPlayer();
        /* dist = |player.pos - pos| (xyz) */
        diff[0] = player->base.pos[0] - pos[0];
        diff[1] = player->base.pos[1] - pos[1];
        diff[2] = player->base.pos[2] - pos[2];
        dist = __builtin_sqrtf(diff[0] * diff[0] + diff[1] * diff[1] + diff[2] * diff[2]);
        vol = (300.0f - dist) * 0.0033333334f;
        if (!(vol <= 1.0f)) {
          vol = 1.0f;
        }
        if (!(vol <= 0.0f)) {
          cam = (GameFieldCamera *)gf->camera;
          cam->shakeTimer = (s16)(g_bgBattleWildaTimer >> 1);
          cam->shakeAmplitude = vol;
          SndEmitterCreateAtPos(SndGetListener(), 0x2c000c9, pos, 0, 1);
        }
        break;
      }
    }
  }

  if (g_bgBattleWildaTimer != 0) {
    g_bgBattleWildaTimer = g_bgBattleWildaTimer - 1;
    if (g_bgBattleWildaTimer == 0) {
      for (actor = *(Actor **)ActorGetList(); actor != NULL;
           actor = (Actor *)actor->base.base.next) {
        if (actor->base.base.unk08 == 7) {
          GfxModelPlayMotionByName(0.2f, &actor->base, g_bgBattleMotionNames[0], true);
        }
      }
    }
  }

  if (g_bgBattleFortressTimer != 0) {
    g_bgBattleFortressTimer = g_bgBattleFortressTimer - 1;
    if (g_bgBattleFortressTimer == 0) {
      for (actor = *(Actor **)ActorGetList(); actor != NULL;
           actor = (Actor *)actor->base.base.next) {
        if (actor->base.base.unk08 == 0x1f) {
          GfxModelPlayMotionByName(0.2f, &actor->base, g_bgBattleMotionNames[2], true);
        }
      }
    }
  }

  g_bgBattleFrame = g_bgBattleFrame + 1;
  if (g_bgBattleFrame >= 0x97) {
    g_bgBattleFrame = 0;
  }
}
