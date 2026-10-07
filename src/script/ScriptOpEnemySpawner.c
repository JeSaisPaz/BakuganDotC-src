// bdc 0x08810b54 ScriptOpEnemySpawner
#include "bdc.h"

/* Script opcode that drives the enemy spawners (`g_btlEnemySpawnerList`, `BtlEnemySpawnerCtor`).
   Operands: u32 `mode`, u32 `id`, floats `on`, `spawnerId`, plus 2 unused floats. Mode 0: `on > 0`
   (or NaN) creates a spawner (`BtlEnemySpawnerCreate``((int)spawnerId)`), otherwise destroys those
   with that id (`BtlEnemySpawnerDestroyById`). Mode 1: sets the enable flag of the spawner whose id
   equals `id` (the last spawner in the list if none matches) to `!(on <= 0)`. Returns 0. */

int ScriptOpEnemySpawner(Script *script)

{
  s32 mode;
  s32 id;
  float on;
  float spawnerId;
  BtlEnemySpawner *spawner;
  BtlEnemySpawner *next;

  mode = (s32)ScriptReadU32(script);
  id = (s32)ScriptReadU32(script);
  on = ScriptReadFloat(script);
  spawnerId = ScriptReadFloat(script);
  ScriptReadFloat(script);
  ScriptReadFloat(script);
  spawner = (BtlEnemySpawner *)0x0;
  if (g_btlEnemySpawnerList != (CoreObjectList *)0x0) {
    spawner = (BtlEnemySpawner *)g_btlEnemySpawnerList->head;
    if (spawner != (BtlEnemySpawner *)0x0) {
      /* stops on a match or at the last node (which is kept even if its id differs) */
      next = (BtlEnemySpawner *)spawner->base.next;
      while (spawner->id != id && next != (BtlEnemySpawner *)0x0) {
        spawner = next;
        next = (BtlEnemySpawner *)spawner->base.next;
      }
    }
  }
  if (mode < 1) {
    if (mode >= 0) {
      if (on <= 0.0f) {
        BtlEnemySpawnerDestroyById((int)spawnerId);
      }
      else {
        BtlEnemySpawnerCreate((int)spawnerId);
      }
    }
  }
  else if (mode < 2 && spawner != (BtlEnemySpawner *)0x0) {
    if (on <= 0.0f) {
      spawner->enabled = 0;
    }
    else {
      spawner->enabled = 1;
    }
  }
  return 0;
}
