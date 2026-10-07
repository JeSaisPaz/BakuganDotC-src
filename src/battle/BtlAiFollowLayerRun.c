// bdc 0x08895210 BtlAiFollowLayerRun
#include "bdc.h"

/* Run method of behaviour layer 1 of `BtlAi` (follow an ally, `MemberFnPtr` pair at
   `0x08a802f0`, check `BtlAiFollowLayerCheck`): ticks the layer timer (`+0x6c` limit, `+0x70`
   elapsed, `+0x74` expired) and steps the state `+0x60`: 0 sets the 'ignore targets' bit 0 of
   `+0x68` with a 1-s timer and falls into 1, which sets a goal at a random point within
   `g_btlAiRangeConsts.followScatter` of the ally `+0x9c` (`CoreRandFloat`, `CoreRandAngle`, `BtlAiSetGoalPoint`);
   2 stays while `BtlAiChannelIsFinished` returns 1 for the move channel `+0x2d8` (set while the
   goal move is under way, cleared when `BtlAiMoveToGoalPoint` resets the channel), then clears
   bit 0 and restarts the 1-s timer (state 3); 3 waits for the timer and re-picks a point (state 1).
   State 2 ends (state 4) when bit 0 is clear and a target exists or the ally's byte `+0x4c1` is
   clear, state 3 on the same condition regardless of bit 0; 4 clears the layer (`+0x60..+0x9c`).
   Unless bit 0 is set it runs `BtlAiUpdateTarget` (resetting the move channel on a new target),
   and it always steers with `BtlAiMoveToGoalPoint`. The random offset is
   `(cos a, 0, sin a) * dist` (`vrot.q` with the bank's 2/pi). */

void BtlAiFollowLayerRun(BtlAi *self)
{
    BtlAiLayer *layer = &self->follow.base;
    float point[4];
    float *leaderPos;
    float dist;
    float angle;

    if (!layer->timerExpired) {
        layer->timerElapsed = layer->timerElapsed + 0.0333333351f;
        if (!(layer->timerElapsed < layer->timerLimit)) {
            layer->timerElapsed = layer->timerLimit;
            layer->timerExpired = 1;
        }
    }

    switch ((u32)layer->state) {
    case 0:
        /* ignore targets while walking to the first point; 1-s timer */
        layer->flags |= 1;
        layer->timerLimit = 1.0f;
        layer->timerElapsed = 0.0f;
        layer->timerExpired = 0;
        layer->state = layer->state + 1;
        /* fall through */
    case 1:
        dist = CoreRandFloat(g_btlAiRangeConsts.followScatter);
        angle = CoreRandAngle();
        /* point.xyz = (cos a, 0, sin a) * dist + leader pos; w = 0 from the rotation */
        leaderPos = self->follow.leader->base.pos;
        point[0] = __builtin_cosf(angle) * dist + leaderPos[0];
        point[1] = 0.0f * dist + leaderPos[1];
        point[2] = __builtin_sinf(angle) * dist + leaderPos[2];
        point[3] = 0.0f;
        BtlAiSetGoalPoint(self, point);
        layer->state = layer->state + 1;
        break;
    case 2:
        if (layer->timerExpired) {
            layer->flags &= ~1u;
        }
        if ((layer->flags & 1) == 0
            && (self->target != NULL || !self->follow.leader->combat.dead)) {
            layer->state = 4;
        } else if (BtlAiChannelIsFinished(self->channels) == 0) {
            layer->flags &= ~1u;
            layer->timerLimit = 1.0f;
            layer->timerElapsed = 0.0f;
            layer->timerExpired = 0;
            layer->state = layer->state + 1;
        }
        break;
    case 3:
        if (self->target != NULL || !self->follow.leader->combat.dead) {
            layer->state = 4;
        } else if (layer->timerExpired) {
            layer->state = 1;
        }
        break;
    case 4:
        layer->state = 0;
        layer->active = 0;
        layer->flags = 0;
        layer->cmd.state = 0;
        layer->cmd.timerLimit = 0.0f;
        layer->cmd.timerElapsed = 0.0f;
        layer->cmd.timerExpired = 1;
        layer->cmd.flags = 0;
        layer->cmd.arg = 0;
        layer->cmd.argF = 0.0f;
        layer->cmd.finished = 0;
        layer->cmd.failed = 0;
        self->follow.leader = NULL;
        return;
    default:
        break;
    }

    if ((layer->flags & 1) == 0 && BtlAiUpdateTarget(self) != 0) {
        BtlAiChannelReset(self->channels);
    }
    BtlAiMoveToGoalPoint(self);
}
