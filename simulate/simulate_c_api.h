#include "simulate.h"
extern "C" {
    using Sim = mujoco::Simulate;
    mujoco::Simulate* mujoco_Simulate_create(mjvCamera* cam, mjvOption* opt, mjvPerturb* pert, mjvScene* user_scn, bool is_passive);
    void mujoco_Simulate_destroy(Sim* sim);
    void mujoco_Simulate_RenderInit(Sim* sim);
    void mujoco_Simulate_RenderStep(Sim* sim, bool update_timer);
    void mujoco_Simulate_RenderCleanup(Sim* sim);
    void mujoco_Simulate_Sync(Sim* sim, bool stateonly);
}
