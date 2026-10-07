// bdc 0x088a588c BtlEnemySpawnerUpdate
#include "bdc.h"

/* Per-frame step of a `BtlEnemySpawner`, idle once `spawnCount` reaches 5. State 0 (while
   `enabled`): counts `timer` down; at 0 it spawns (`BtlEnemySpawnerSpawn`, state 1) when none
   of its units is alive (`BtlEnemySpawnerCountAlive`), else re-checks in 5 frames. State 1:
   counts down; at 0 goes to state 2 once no unit is alive, else re-checks in 5 frames. State 2
   (while `enabled`): counts down; at 0 spawns again (state 1) when no unit is alive, else waits
   150 frames. Negative or larger states do nothing. */
void BtlEnemySpawnerUpdate(void *spawner)
{
    BtlEnemySpawner *self = spawner;

    if (self->spawnCount >= 5) {
        return;
    }
    switch (self->state) {
    case 0:
        if (self->enabled == 0) {
            return;
        }
        if (--self->timer > 0) {
            return;
        }
        self->timer = 0;
        if (BtlEnemySpawnerCountAlive(self) < 1) {
            self->state = 1;
            BtlEnemySpawnerSpawn(self);
        } else {
            self->timer = 5;
        }
        break;
    case 1:
        if (--self->timer > 0) {
            return;
        }
        self->timer = 0;
        if (BtlEnemySpawnerCountAlive(self) > 0) {
            self->timer = 5;
        } else {
            self->state = 2;
        }
        break;
    case 2:
        if (self->enabled == 0) {
            return;
        }
        if (--self->timer > 0) {
            return;
        }
        if (BtlEnemySpawnerCountAlive(self) < 1) {
            self->state = 1;
            BtlEnemySpawnerSpawn(self);
        } else {
            self->timer = 150;
        }
        break;
    }
}
