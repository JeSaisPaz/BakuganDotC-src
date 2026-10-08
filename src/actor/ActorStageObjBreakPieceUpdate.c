// bdc 0x088afaec ActorStageObjBreakPieceUpdate
#include "bdc.h"

/* Update method of the debris piece (vtable `0x08af29a4` slot 7, `ActorStageObjBreakPieceCtor`):
   distance fade into `+0x330` (always translucent-capable) and draw alpha `+0x6c = +0x330 *
   +0x338`, then runs the per-piece handler of table `0x08a84c3c` indexed by the piece `+0x324`: 0
   `ActorStageObjBreakPieceRemove`, 1/2 building/warehouse collapse
   (`ActorStageObjBreakPieceCollapse01`, `ActorStageObjBreakPieceCollapse02`), 3 crane collapse
   (`ActorStageObjBreakPieceCraneCollapse`), 4..6 rubble (`ActorStageObjBreakPieceRubbleIdle`);
   finally `ActorStageObjRebuildMatrix`. */

void ActorStageObjBreakPieceUpdate(ActorStageObjBreakPiece *self)
{
  int hidden;

  hidden = ActorStageObjUpdateFade((self->base).diagonal2 * 5.0f, 4000.0f, self, &self->base,
                                   &self->fade, (char *)&self->fadeState, 1);
  self->hidden = hidden == 0;
  (self->base).base.ambient[3] = self->fade * self->baseAlpha;
  if (self->piece >= 0 && self->piece < 7) {
    const VtblEntry *entry = &g_actorStageObjBreakPieceStateTable[self->piece];
    u8 *obj = (u8 *)self + entry->delta;
    void (*fn)(void *) = (void (*)(void *))entry->fn;

    if (entry->pad != 0) {
      const VtblEntry *v = (const VtblEntry *)*(void **)(obj + (intptr_t)fn) + entry->pad;
      fn = (void (*)(void *))v->fn;
      obj += v->delta;
    }
    fn(obj);
  }
  ActorStageObjRebuildMatrix(&self->base, 0);
}
