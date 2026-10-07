// bdc 0x088d9ea4 GameGimmickTriggerZoneCtor
#include "bdc.h"

/* Constructor of a box-shaped trigger gimmick (0x270 bytes, spawned by `GameFieldLoadObjects`
   with kind 0): `GameGimmickCtor`, vtables `g_gameGimmickTriggerZoneVtbl` /
   `g_gameGimmickTriggerZoneVtbl2`, then sets up the embedded `CollisionBox` `box` (type 6,
   `g_collisionBoxVtbl`): clears `state`, `ambient[3]`, `attached` and `drawn`; the box extents are
   -5 / +5 times the record `extent` (20.12), scaled by 1.3 when the record type is 0x322, and its
   transform is the Y rotation by `rot[1]` with translation `pos`, w = 1; the inverse (transposed
   rotation, negated rotated translation) is computed inline and `invValid` set.
   Finally clears `effect` and `inside` and sets `color` to (1, 1, 1, 1). Returns `obj`.
   The extents' w lanes are not written here (the original stores a stale VFPU lane there). */

CoreObject *GameGimmickTriggerZoneCtor(GameGimmickTriggerZone *obj, s32 kind, void *record, u16 typeId, u8 flag)
{
  GameGimmickRecord *rec;
  ScePspFMatrix4 *t;
  ScePspFMatrix4 *inv;
  float ex;
  float ey;
  float ez;
  float lo[3];
  float hi[3];
  float c;
  float s;
  float d0;
  float d1;
  float d2;

  GameGimmickCtor(&obj->base, kind, record, typeId, flag);
  obj->base.base.base.vtable = g_gameGimmickTriggerZoneVtbl;
  obj->base.vtbl2 = g_gameGimmickTriggerZoneVtbl2;
  obj->box.vtbl = g_collisionBoxVtbl;
  obj->box.invValid = 0;
  obj->box.type = 6;
  obj->base.state = 0;
  obj->base.base.ambient[3] = 0.0f;
  obj->base.attached = NULL;
  obj->drawn = 0;

  rec = (GameGimmickRecord *)obj->base.record;
  ex = (float)rec->extent[0] * 0.000244140625f;
  ey = (float)rec->extent[1] * 0.000244140625f;
  ez = (float)rec->extent[2] * 0.000244140625f;
  lo[0] = -5.0f * ex;
  lo[1] = -5.0f * ey;
  lo[2] = -5.0f * ez;

  rec = (GameGimmickRecord *)obj->base.record;
  ex = (float)rec->extent[0] * 0.000244140625f;
  ey = (float)rec->extent[1] * 0.000244140625f;
  ez = (float)rec->extent[2] * 0.000244140625f;
  hi[0] = 5.0f * ex;
  hi[1] = 5.0f * ey;
  hi[2] = 5.0f * ez;

  /* Y rotation (vrot of rot[1] times 2/pi). */
  c = __builtin_cosf(obj->base.base.rot[1]);
  s = __builtin_sinf(obj->base.base.rot[1]);

  rec = (GameGimmickRecord *)obj->base.record;
  if (rec->recordType == 0x322) {
    lo[0] = lo[0] * 1.3f;
    lo[1] = lo[1] * 1.3f;
    lo[2] = lo[2] * 1.3f;
    hi[0] = hi[0] * 1.3f;
    hi[1] = hi[1] * 1.3f;
    hi[2] = hi[2] * 1.3f;
  }

  obj->box.aabbMin.x = lo[0];
  obj->box.aabbMin.y = lo[1];
  obj->box.aabbMin.z = lo[2];
  obj->box.aabbMax.x = hi[0];
  obj->box.aabbMax.y = hi[1];
  obj->box.aabbMax.z = hi[2];
  obj->box.invValid = 0;

  t = &obj->box.transform;
  t->x.x = c;
  t->x.y = 0.0f;
  t->x.z = -s;
  t->x.w = 0.0f;
  t->y.x = 0.0f;
  t->y.y = 1.0f;
  t->y.z = 0.0f;
  t->y.w = 0.0f;
  t->z.x = s;
  t->z.y = 0.0f;
  t->z.z = c;
  t->z.w = 0.0f;
  t->w.x = obj->base.base.pos[0];
  t->w.y = obj->base.base.pos[1];
  t->w.z = obj->base.base.pos[2];
  t->w.w = 1.0f;

  if (obj->box.invValid == 0) {
    inv = &obj->box.invTransform;
    d0 = t->x.x * t->w.x + t->x.y * t->w.y + t->x.z * t->w.z;
    d1 = t->y.x * t->w.x + t->y.y * t->w.y + t->y.z * t->w.z;
    d2 = t->z.x * t->w.x + t->z.y * t->w.y + t->z.z * t->w.z;
    inv->x.x = t->x.x;
    inv->x.y = t->y.x;
    inv->x.z = t->z.x;
    inv->x.w = 0.0f;
    inv->y.x = t->x.y;
    inv->y.y = t->y.y;
    inv->y.z = t->z.y;
    inv->y.w = 0.0f;
    inv->z.x = t->x.z;
    inv->z.y = t->y.z;
    inv->z.z = t->z.z;
    inv->z.w = 0.0f;
    inv->w.x = -d0;
    inv->w.y = -d1;
    inv->w.z = -d2;
    inv->w.w = t->w.w;
    obj->box.invValid = 1;
  }

  obj->effect = NULL;
  obj->color.x = 1.0f;
  obj->color.y = 1.0f;
  obj->color.z = 1.0f;
  obj->color.w = 1.0f;
  obj->inside = 0;
  return &obj->base.base.base;
}
