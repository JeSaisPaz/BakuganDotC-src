// bdc 0x088a5604 BtlEnemySpawnerCreate
#include "bdc.h"

/* Allocates a 0x40-byte `BtlEnemySpawner` from the low end of the heap (the previous placement
   policy is restored afterwards, all under `MemLock`), runs `BtlEnemySpawnerCtor` on it, stores
   the spawner/group `id` and returns it. Called by `ScriptOpEnemySpawner` (mode 0, positive flag);
   counterpart `BtlEnemySpawnerDestroyById`. The `id` store is unconditional: when the
   allocation fails the original writes through a NULL spawner. */
void *BtlEnemySpawnerCreate(int id)
{
  bool fromLow;
  BtlEnemySpawner *spawner;
  BtlEnemySpawner *result;

  MemLock();
  fromLow = MemIsAllocFromLow();
  MemSetAllocFromLow(true);
  spawner = (BtlEnemySpawner *)MemAlloc(sizeof(BtlEnemySpawner), NULL, 0);
  MemSetAllocFromLow(fromLow);
  MemUnlock();
  result = NULL;
  if (spawner != NULL) {
    BtlEnemySpawnerCtor(spawner);
    result = spawner;
  }
  result->id = id;
  return result;
}
