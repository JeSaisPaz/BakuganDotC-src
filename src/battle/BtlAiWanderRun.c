// bdc 0x08895ad0 BtlAiWanderRun
#include "bdc.h"

/* Run method of the wander behaviour layer (`wander`) of `BtlAi`: advances the layer
   timer (`timerElapsed` += 1/30 s until it reaches `timerLimit`, then `timerExpired`), then runs a
   small state machine on `state`: 0 sets bit 0 of `flags`, arms a 1 s timer and falls into 1;
   1 picks a random goal within 2000 units of the `home` point (`CoreRandFloat` radius,
   `CoreRandAngle` heading) and starts moving there (`BtlAiSetGoalPoint`);
   2 clears bit 0 once the timer expired and, while the move channel is not finished
   (`BtlAiChannelIsFinished`), clears bit 0, arms a 0.5 s pause and advances; 3 goes back to 1
   once the pause expired; other states do nothing. Afterwards it looks for targets
   (`BtlAiUpdateTarget`) while bit 0 of `flags` is clear, always steers with
   `BtlAiMoveToGoalPoint` and, once `target` is set, calls the layer's Reset virtual (vtable
   entry 1). */

void BtlAiWanderRun(BtlAi *self)
{
    BtlAiLayer *layer = &self->wander.base;
    float goal[4] __attribute__((aligned(16)));
    float elapsed;
    float radius;
    float angle;
    s32 state;
    const VtblEntry *reset;

    if (layer->timerExpired == 0) {
        elapsed = layer->timerElapsed + 0.0333333351f;
        layer->timerElapsed = elapsed;
        if (!(elapsed < layer->timerLimit)) {
            layer->timerElapsed = layer->timerLimit;
            layer->timerExpired = 1;
        }
    }
    state = layer->state;
    if (state == 0 || state == 1) {
        if (state == 0) {
            layer->state = layer->state + 1;
            layer->flags = layer->flags | 1;
            layer->timerElapsed = 0.0f;
            layer->timerLimit = 1.0f;
            layer->timerExpired = 0;
        }
        radius = CoreRandFloat(2000.0f);
        angle = CoreRandAngle();
        /* goal.xyz = home.xyz + radius * (cos, 0, sin)(angle); goal.w = 0 */
        goal[0] = __builtin_cosf(angle) * radius;
        goal[1] = 0.0f * radius;
        goal[2] = __builtin_sinf(angle) * radius;
        goal[3] = 0.0f;
        goal[0] = goal[0] + self->wander.home[0];
        goal[1] = goal[1] + self->wander.home[1];
        goal[2] = goal[2] + self->wander.home[2];
        BtlAiSetGoalPoint(self, goal);
        layer->state = layer->state + 1;
    } else if (state == 2) {
        if (layer->timerExpired != 0) {
            layer->flags = layer->flags & ~1u;
        }
        if (BtlAiChannelIsFinished(&self->channels[0]) == 0) {
            layer->flags = layer->flags & ~1u;
            layer->timerLimit = 0.5f;
            layer->timerElapsed = 0.0f;
            layer->timerExpired = 0;
            layer->state = layer->state + 1;
        }
    } else if (state == 3) {
        if (layer->timerExpired != 0) {
            layer->state = 1;
        }
    }
    if ((layer->flags & 1) == 0) {
        BtlAiUpdateTarget(self);
    }
    BtlAiMoveToGoalPoint(self);
    if (self->target != NULL) {
        reset = &layer->vtbl[1];
        ((void (*)(void *))reset->fn)((u8 *)layer + reset->delta);
    }
}
