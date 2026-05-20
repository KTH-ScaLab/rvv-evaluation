#include "pybind_main_rvv.h"

#include "../../lib/formux.h"
#include "../../lib/simulator_rvv.h"
#include "../../lib/util_cpu.h"

namespace qsim {
  template <typename For>
  using Simulator = SimulatorRVV<For>;

  struct Factory {
    // num_state_threads and num_dblocks are unused, but kept for consistency
    // with the GPU Factory.
    Factory(
      unsigned num_sim_threads,
      unsigned num_state_threads,
      unsigned num_dblocks) : num_threads(num_sim_threads) {}

    using Simulator = qsim::Simulator<For>;
    using StateSpace = Simulator::StateSpace;

    StateSpace CreateStateSpace() const {
      return StateSpace(num_threads);
    }

    Simulator CreateSimulator() const {
      return Simulator(num_threads);
    }

    unsigned num_threads;
  };
}

#include "../pybind_main.cpp"
