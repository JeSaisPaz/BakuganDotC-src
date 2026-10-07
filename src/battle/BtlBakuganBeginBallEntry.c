// bdc 0x0889ab14 BtlBakuganBeginBallEntry
#include "bdc.h"

/* Arms the ball entry sequence of a freshly spawned CPU unit (`BtlBakuganRunBallEntry`): stores
   `pending` in `ballEntryPending` (non-zero = entry to run), resets the entry step `ballEntryStep`
   and makes the unit invisible (ambient alpha 0). When script variable 8 is 1 it also sets the
   colliders to layer 5 (`BtlBakuganSetColliderLayer`) and, when the species' ball model
   `species + 0x57` (`*_P.gmo`) is loaded (`BtlModelExists`), creates it
   (`ActorBallCreate``(1.0, species + 0x57, 3, NULL)`) as `ballModel`, sets its ambient alpha to 0,
   calls its vtable entry 6 with 0.0 and applies the per-copy texture variant of the unit
   (`BtlBakuganCountEarlierSameSpecies`, `ActorBallMaybeApplyTextureVariant`). */

void BtlBakuganBeginBallEntry(BtlBakugan *self, int pending)
{
    BtlCpuUnit *unit = (BtlCpuUnit *)self;
    u32 ballKind;
    GfxModel *ball;
    const VtblEntry *entry;

    self->ballEntryPending = pending;
    unit->ballEntryStep = 0;
    self->base.ambient[3] = 0.0f;
    if (g_scriptGlobalVars[8] == 1) {
        BtlBakuganSetColliderLayer(self, 5);
        ballKind = self->base.base.unk08 + 0x57;
        if (BtlModelExists(self, ballKind) != 0) {
            unit->ballModel = (GfxModel *)ActorBallCreate(1.0f, ballKind, 3, NULL);
            unit->ballModel->ambient[3] = 0.0f;
            ball = unit->ballModel;
            entry = &((const VtblEntry *)ball->base.vtable)[6];
            ((void (*)(void *, float))entry->fn)((u8 *)ball + entry->delta, 0.0f);
            ball = unit->ballModel;
            ActorBallMaybeApplyTextureVariant(ball, BtlBakuganCountEarlierSameSpecies(self));
        }
    }
}
