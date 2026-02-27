#ifndef MUJOCO_SIMULATE_SIMULATE_C_H_
#define MUJOCO_SIMULATE_SIMULATE_C_H_

#include <mujoco/mujoco.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mujoco_Simulate mujoco_Simulate;

mujoco_Simulate* mujoco_cSimulate_create(
    mjvCamera* cam,
    mjvOption* opt,
    mjvPerturb* pert,
    mjvScene* user_scn
);

void mujoco_cSimulate_RenderInit(mujoco_Simulate* sim);

void mujoco_cSimulate_Load(
    mujoco_Simulate* sim, 
    mjModel* m, 
    mjData* d, 
    const char* displayed_filename
);

int mujoco_cSimulate_RenderStep(mujoco_Simulate* sim);

void mujoco_cSimulate_Sync(mujoco_Simulate* sim, int state_only);

void mujoco_cSimulate_ExitRequest(mujoco_Simulate* sim);

void mujoco_cSimulate_destroy(mujoco_Simulate* sim);

#ifdef __cplusplus
}
#endif

#endif  // MUJOCO_SIMULATE_SIMULATE_C_H_
