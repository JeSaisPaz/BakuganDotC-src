// bdc 0x088ac9cc ActorStageObjEnsureHpGauge
#include "bdc.h"

/* Lazily creates the HUD hit-point gauge of a stage object: when `hpGauge` is NULL it reads the
   translation of the model matrix (`rootMatrix[12..15]` of the model at `base.data`), snaps it to
   the ground with `CollisionFindGroundPoint` (mask `0x3fbf2100`), stores the result in
   `gaugePos` with `gaugePos[1]` raised by 100.0 (the gauge's world anchor), allocates 0xa0 bytes
   from the low heap, builds the gauge with `UiHpGaugeCtorForObject``(gauge, obj)` and stores it
   in `hpGauge`. */

void ActorStageObjEnsureHpGauge(ActorStageObjBase *self)
{
  float ground[4];
  float pos[4];
  float *src;
  int i;
  bool fromLow;
  UiHpGauge *gauge;
  UiHpGauge *result;

  if (self->hpGauge == NULL) {
    src = &self->base.data->rootMatrix[12];
    for (i = 0; i < 4; i++) {
      pos[i] = src[i];
    }
    CollisionFindGroundPoint(ground, pos, 0x3fbf2100);
    for (i = 0; i < 4; i++) {
      pos[i] = ground[i];
    }
    for (i = 0; i < 4; i++) {
      self->gaugePos[i] = pos[i];
    }
    self->gaugePos[1] = self->gaugePos[1] + 100.0f;
    result = NULL;
    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    gauge = MemAlloc(sizeof(UiHpGauge), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    if (gauge != NULL) {
      UiHpGaugeCtorForObject(gauge, self);
      result = gauge;
    }
    self->hpGauge = result;
  }
}
