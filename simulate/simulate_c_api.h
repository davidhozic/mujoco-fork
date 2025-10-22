#include "simulate.h"
extern "C" {
    using Sim = mujoco::Simulate;
    mujoco::Simulate* mujoco_cSimulate_create(mjvCamera* cam, mjvOption* opt, mjvPerturb* pert, mjvScene* user_scn, bool is_passive);
    void mujoco_cSimulate_destroy(Sim* sim);
    void mujoco_cSimulate_RenderInit(Sim* sim);
    uint8_t mujoco_cSimulate_RenderStep(Sim* sim, bool update_timer);
    void mujoco_cSimulate_RenderCleanup(Sim* sim);
    void mujoco_cSimulate_Sync(Sim* sim, bool stateonly);
    void mujoco_cSimulate_Load(Sim* sim, mjModel* m, mjData* d, const char* displayed_filename);
}
