#include "simulate_c_api.h"
#include <stdint.h>

extern "C" {
    Sim* mujoco_cSimulate_create(mjvCamera* cam, mjvOption* opt, mjvPerturb* pert, mjvScene* user_scn, bool is_passive) {
        return new Sim(cam, opt, pert, user_scn, is_passive);
    }
    void mujoco_cSimulate_destroy(Sim* sim) {
        delete sim;
    }
    void mujoco_cSimulate_RenderInit(Sim* sim) { sim->RenderInit(); }
    uint8_t mujoco_cSimulate_RenderStep(Sim* sim, bool update_timer) { return (uint8_t) sim->RenderStep(update_timer); }
    void mujoco_cSimulate_RenderCleanup(Sim* sim) { sim->RenderCleanup(); }
    void mujoco_cSimulate_Sync(Sim* sim, bool stateonly) { sim->Sync(stateonly); }
    void mujoco_cSimulate_Load(Sim* sim, mjModel* m, mjData* d, const char* displayed_filename) {
        sim->Load(m, d, displayed_filename);
    }
}
