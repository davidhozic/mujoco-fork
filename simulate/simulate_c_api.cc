#include "simulate_c_api.h"

extern "C" {
    Sim* mujoco_Simulate_create(mjvCamera* cam, mjvOption* opt, mjvPerturb* pert, mjvScene* user_scn, bool is_passive) {
        return new Sim(cam, opt, pert, user_scn, is_passive);
    }
    void mujoco_Simulate_destroy(Sim* sim) {
        delete sim;
    }
    void mujoco_Simulate_RenderInit(Sim* sim) { sim->RenderInit(); }
    void mujoco_Simulate_RenderStep(Sim* sim, bool update_timer) { sim->RenderStep(update_timer); }
    void mujoco_Simulate_RenderCleanup(Sim* sim) { sim->RenderCleanup(); }
    void mujoco_Simulate_Sync(Sim* sim, bool stateonly) { sim->Sync(stateonly); }
}
