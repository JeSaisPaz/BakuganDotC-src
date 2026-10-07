// bdc 0x088d579c GameGimmickCollidableUpdate
#include "bdc.h"

/* Update (vtable slot 7) of the breakable container gimmick (`GameGimmickCollidableCtor`, vtables
   `0x08af2e2c`/`0x08af2ecc`): runs `GameGimmickUpdate`; when its collider reports a ball hit
   (`hitCollider->flags` bit 0x10): if it is not yet broken and the record has contents
   (`hasContents`), plays sound `0x2c00031` and spawns 1 or 3 core points (`GameGimmickCorePointCtor`,
   kind 10/11/12 = S/M/L from record `contentKind`, type 0x1778, record copy `i`) launched along the
   direction from the collider hit position to the player (`GameGimmickCorePointLaunch`), starting at
   the gimmick position, counting each on the field HUD (`UiFieldHudIncrementCounter`, task 3001);
   then marks it broken. On every hit it snaps the player's ball `vec160` to the gimmick position,
   clears the hit bit and enters state 1. Then runs the current state from
   `g_gimmickCollidableStateTable` (state 1 = `GameGimmickCollidableStateHit`) and steps the motion;
   when the motion has at most one frame left at speed 1, rewinds it to frame 0 and calls virtual slot 6
   (`+0x30`) with 0.0f.
   Reads the VFPU bank constants C720 (0, initial value of the stack temporaries, dead) and S713
   (0: scale when the hit-to-player vector has zero length, and dir.w). */

void GameGimmickCollidableUpdate(GameGimmickCollidable *obj)
{
  ActorPlayer *player;
  CollisionCollider *col;

  GameGimmickUpdate(&obj->base);
  player = (ActorPlayer *)ActorFindPlayer();
  col = (CollisionCollider *)obj->hitCollider;
  if ((col->flags & 0x10) != 0) {
    if (obj->broken == 0) {
      GameGimmickRecord *rec = (GameGimmickRecord *)obj->base.record;

      if (rec->hasContents != 0) {
        u8 kind = 10;
        u8 count = 1;
        s32 i;

        switch (rec->contentKind) {
        case 1: kind = 11; count = 1; break;
        case 2: kind = 12; count = 1; break;
        case 3: kind = 10; count = 3; break;
        case 4: kind = 11; count = 3; break;
        case 5: kind = 12; count = 3; break;
        default: break; /* 0 and >= 6: one S core point */
        }
        if (SndHasManager()) {
          SndManagerPlay(SndGetManager(), 0x2c00031, 0, 0);
        }
        for (i = 0; i < (s32)count; i++) {
          ScePspFVector4 dir;      /* sp+0x10 */
          ScePspFVector4 start;    /* sp+0x20 */
          ScePspFVector4 objPos;   /* sp+0x50 */
          ScePspFVector4 hitPos;   /* sp+0x60 */
          ScePspFVector4 playerPos; /* sp+0x70 */
          ScePspFVector4 diff;     /* sp+0x80 */
          UiFieldHud *hud;
          GameGimmickCorePoint *cp;
          bool fromLow;
          float lenSq;
          float scale;

          hud = (UiFieldHud *)CoreTaskFind(0xbb9);
          if (hud != NULL) {
            UiFieldHudIncrementCounter(hud);
          }
          MemLock();
          fromLow = MemIsAllocFromLow();
          MemSetAllocFromLow(true);
          cp = (GameGimmickCorePoint *)MemAlloc(0x2e0, NULL, 0);
          MemSetAllocFromLow(fromLow);
          MemUnlock();
          if (cp != NULL) {
            GameGimmickCorePointCtor(cp, kind, obj->recordCopy[i], 0x1778, 0);
          }

          /* the stack temporaries start as the bank zero vector C720 and are then overwritten */
          objPos.x = obj->base.base.pos[0];
          objPos.y = obj->base.base.pos[1];
          objPos.z = obj->base.base.pos[2];
          objPos.w = obj->base.base.pos[3];
          playerPos.x = player->base.base.pos[0];
          playerPos.y = player->base.base.pos[1];
          playerPos.z = player->base.base.pos[2];
          playerPos.w = player->base.base.pos[3];
          col = (CollisionCollider *)obj->hitCollider;
          hitPos.x = col->hitPos.x;
          hitPos.y = col->hitPos.y;
          hitPos.z = col->hitPos.z;
          hitPos.w = col->hitPos.w;
          /* dir.xyz = normalize(playerPos - hitPos), clamped to [-1, 1] (0 when zero length);
             dir.w = S713 = 0; start = objPos */
          diff.x = playerPos.x - hitPos.x;
          diff.y = playerPos.y - hitPos.y;
          diff.z = playerPos.z - hitPos.z;
          lenSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;
          scale = VfRsq(lenSq);
          if (lenSq == 0.0f) {
            scale = 0.0f;
          }
          dir.x = VfSat1(diff.x * scale);
          dir.y = VfSat1(diff.y * scale);
          dir.z = VfSat1(diff.z * scale);
          dir.w = 0.0f;
          start = objPos;
          GameGimmickCorePointLaunch(cp, i, &dir.x, &start.x);
        }
      }
      obj->broken = 1;
    }
    {
      ActorBall *ball = (ActorBall *)player->ball;

      ball->vec160[0] = obj->base.base.pos[0];
      ball->vec160[1] = obj->base.base.pos[1];
      ball->vec160[2] = obj->base.base.pos[2];
      ball->vec160[3] = obj->base.base.pos[3];
    }
    col = (CollisionCollider *)obj->hitCollider;
    col->flags &= ~0x10u;
    obj->base.state = 1;
  }

  if (obj->base.state >= 0 && (u32)obj->base.state < 2) {
    const VtblEntry *entry = &g_gimmickCollidableStateTable[obj->base.state];
    u8 *self = (u8 *)obj + entry->delta;
    void *fn = entry->fn;

    if (entry->pad != 0) {
      const VtblEntry *vt = *(const VtblEntry **)(self + (intptr_t)fn);

      vt += entry->pad;
      fn = vt->fn;
      self += vt->delta;
    }
    ((void (*)(void *))fn)(self);
  }

  GfxModelUpdateMotion((GfxModel *)obj);
  GfxModelApplyMotion((GfxModel *)obj);
  if (GfxModelGetMotionRemaining((GfxModel *)obj) <= 1.0f &&
      GfxModelGetMotionSpeed((GfxModel *)obj) == 1.0f) {
    const VtblEntry *vt;

    GfxModelSwapMotionFrame((GfxModel *)obj, 0.0f);
    vt = &((const VtblEntry *)obj->base.base.base.vtable)[6];
    ((void (*)(void *, float))vt->fn)((u8 *)obj + vt->delta, 0.0f);
  }
}
