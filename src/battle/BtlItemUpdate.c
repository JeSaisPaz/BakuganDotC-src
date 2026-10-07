// bdc 0x088b6f80 BtlItemUpdate
#include "bdc.h"

/* Per-frame update of a battle pickup item (`BtlItemListUpdateAll`). A despawned item
   (`pickedUp` != 0) is deleted (`BtlItemDespawn`) once `age` reaches `pickedUp + 80`, else only
   gets the tail below. A live one runs its `state`:
   0 plays positional sound 0x200198 (type 0) or 0x200197 (other types) at `pos` when there is a
     listener, then goes to state 5 without `appearAnim`, else on to state 1 this frame;
   1 counts `popFrame`; at 20 goes to state 5, else sets `pos = basePos` with
     `pos[1] += 250 * sin(popFrame * 0.05 * pi * 0.9)`;
   5 waits until `age` >= 31, then goes on to state 10 this frame;
   10 counts `lifetime` down (deleted once it was negative), blinks once it is below 90, and when
     `BtlItemFindPicker` finds a unit stops blinking, goes to state 11 and sets the homing curve
     (`homeYaw` +-pi/2 by the low bit of `g_btlItemListCount`, `homePitch` 0.2pi + random 0.2pi,
     `homeCurve` 1);
   11 flies to the picker's position + 100 up: within distance 80 (squared < 6400) applies the
     pickup (`BtlItemApplyPickup`) and deletes the item; otherwise moves `pos` by `reserved58`
     (+2 per frame below 60, capped at 1.5x the distance) along the direction whose yaw/pitch are
     the direction to the target plus `homeYaw`/`homePitch` times `homeCurve` (which drops 0.03 per
     frame down to 0);
   other states do nothing.
   Tail: while blinking, the glow effect's flag bit 0 is the inverse of `age` bit 0 and the model's
   `visible` is `age` bit 0. A model's root matrix becomes scale 10 times a Y rotation of
   `age * 0.1` rad, translated to `pos`. Then `age` counts up.
   The VFPU trig reads the bank constant S703 (2/pi), so it lifts to sinf/cosf of the radian angle. */

void BtlItemUpdate(void *item)
{
  BtlItem *self = item;
  float dir[4];
  float *root;
  float angle;
  float s;
  float c;
  float sn;
  float distSq;
  float yaw;
  float pitch;
  float cosPitch;
  float cosYaw;
  float sinPitch;
  float sinYaw;
  float limit;
  float curve;
  s32 left;

  if (self->pickedUp != 0) {
    if (self->age >= self->pickedUp + 80) {
      BtlItemDespawn(self);
      return;
    }
    goto tail;
  }
  switch ((u32)self->state) {
  case 0:
    if (self->type == 0) {
      if (SndHasListener()) {
        SndEmitterCreateAtPos(SndGetListener(), 0x200198, self->pos, 0, 1);
      }
    } else {
      if (SndHasListener()) {
        SndEmitterCreateAtPos(SndGetListener(), 0x200197, self->pos, 0, 1);
      }
    }
    if (self->appearAnim == 0) {
      self->state = 5;
      break;
    }
    self->state = self->state + 1;
    /* fall through */
  case 1:
    self->popFrame = self->popFrame + 1;
    if (self->popFrame >= 20) {
      self->state = 5;
      break;
    }
    self->pos[0] = self->basePos[0];
    self->pos[1] = self->basePos[1];
    self->pos[2] = self->basePos[2];
    self->pos[3] = self->basePos[3];
    angle = (float)self->popFrame * 0.05f * 3.1415927f * 0.9f;
    s = __builtin_sinf(angle);
    self->pos[1] = self->pos[1] + s * 250.0f;
    break;
  case 5:
    if (self->age < 31) {
      break;
    }
    self->state = 10;
    /* fall through */
  case 10:
    left = self->lifetime;
    self->lifetime = left - 1;
    if (left < 0) {
      BtlItemDespawn(self);
      return;
    }
    if (self->lifetime < 90) {
      self->blinking = 1;
    }
    if (BtlItemFindPicker(self) == 0) {
      break;
    }
    self->blinking = 0;
    self->state = self->state + 1;
    if ((g_btlItemListCount & 1) != 0) {
      self->homeYaw = 1.5707964f;
    } else {
      self->homeYaw = -1.5707964f;
    }
    s = CoreRandFloat(0.62831855f);
    self->homeCurve = 1.0f;
    self->homePitch = s + 0.62831855f;
    break;
  case 11:
    dir[0] = self->picker->base.pos[0];
    dir[1] = self->picker->base.pos[1];
    dir[2] = self->picker->base.pos[2];
    dir[3] = self->picker->base.pos[3];
    dir[1] = dir[1] + 100.0f;
    dir[0] = dir[0] - self->pos[0];
    dir[1] = dir[1] - self->pos[1];
    dir[2] = dir[2] - self->pos[2];
    distSq = dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2];
    if (distSq < 6400.0f) {
      BtlItemApplyPickup(self);
      BtlItemDespawn(self);
      return;
    }
    yaw = atan2f(dir[2], dir[0]);
    yaw = yaw + self->homeYaw * self->homeCurve;
    pitch = atan2f(dir[1], __builtin_sqrtf(dir[0] * dir[0] + dir[2] * dir[2]));
    pitch = pitch + self->homePitch * self->homeCurve;
    cosPitch = __builtin_cosf(pitch);
    cosYaw = __builtin_cosf(yaw);
    sinPitch = __builtin_sinf(pitch);
    sinYaw = __builtin_sinf(yaw);
    dir[0] = cosYaw * cosPitch;
    dir[1] = sinPitch;
    dir[2] = sinYaw * cosPitch;
    dir[3] = 0.0f;
    if (self->reserved58 < 60.0f) {
      self->reserved58 = self->reserved58 + 2.0f;
    }
    limit = __builtin_sqrtf(distSq) * 1.5f;
    if (!(self->reserved58 <= limit)) {
      self->reserved58 = limit;
    }
    /* dir.w gets the bank zero S713 (vscl.t C710 stored as a quad); pos.w is kept */
    dir[0] = dir[0] * self->reserved58;
    dir[1] = dir[1] * self->reserved58;
    dir[2] = dir[2] * self->reserved58;
    dir[3] = 0.0f;
    self->pos[0] = self->pos[0] + dir[0];
    self->pos[1] = self->pos[1] + dir[1];
    self->pos[2] = self->pos[2] + dir[2];
    curve = self->homeCurve - 0.03f;
    self->homeCurve = curve;
    if (curve < 0.0f) {
      self->homeCurve = 0.0f;
    }
    break;
  default:
    break;
  }

tail:
  if (self->blinking != 0) {
    if (self->effect != NULL) {
      if ((self->age & 1) != 0) {
        ((GfxEffect *)self->effect)->flags &= ~1u;
      } else {
        ((GfxEffect *)self->effect)->flags |= 1u;
      }
    }
    if (self->model != NULL) {
      self->model->visible = (u8)(self->age & 1);
    }
  }
  if (self->model != NULL) {
    root = self->model->data->rootMatrix;
    angle = (float)self->age * 0.1f;
    c = __builtin_cosf(angle);
    sn = __builtin_sinf(angle);
    /* root = rotY(angle) scaled by diag(10, 10, 10, 1), row 3 = pos */
    root[0] = c * 10.0f;
    root[1] = 0.0f;
    root[2] = -sn * 10.0f;
    root[3] = 0.0f;
    root[4] = 0.0f;
    root[5] = 10.0f;
    root[6] = 0.0f;
    root[7] = 0.0f;
    root[8] = sn * 10.0f;
    root[9] = 0.0f;
    root[10] = c * 10.0f;
    root[11] = 0.0f;
    root = self->model->data->rootMatrix;
    root[12] = self->pos[0];
    root[13] = self->pos[1];
    root[14] = self->pos[2];
    root[15] = self->pos[3];
  }
  self->age = self->age + 1;
}
