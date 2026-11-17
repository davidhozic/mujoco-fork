#ifndef MUJOCO_SIMULATE_SIMULATE_C_H_
#define MUJOCO_SIMULATE_SIMULATE_C_H_

#include <mujoco/mujoco.h>

// The C interface stays visible when the build hides all other names.
#if defined(_WIN32)
#define MJSIMULATE_C_API
#else
#define MJSIMULATE_C_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct mujoco_Simulate mujoco_Simulate;

MJSIMULATE_C_API mujoco_Simulate* mujoco_cSimulate_create(
    mjvCamera* cam,
    mjvOption* opt,
    mjvPerturb* pert,
    mjvScene* user_scn
);

MJSIMULATE_C_API void mujoco_cSimulate_RenderInit(mujoco_Simulate* sim);

MJSIMULATE_C_API void mujoco_cSimulate_Load(
    mujoco_Simulate* sim, 
    mjModel* m, 
    mjData* d, 
    const char* displayed_filename
);

MJSIMULATE_C_API int mujoco_cSimulate_RenderStep(mujoco_Simulate* sim);

MJSIMULATE_C_API void mujoco_cSimulate_Sync(mujoco_Simulate* sim, int state_only);

MJSIMULATE_C_API void mujoco_cSimulate_ExitRequest(mujoco_Simulate* sim);

MJSIMULATE_C_API void mujoco_cSimulate_destroy(mujoco_Simulate* sim);

#ifdef __cplusplus
}
#endif

#endif  // MUJOCO_SIMULATE_SIMULATE_C_H_
