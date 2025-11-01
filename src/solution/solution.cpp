#include "tester.hpp"
#include "solution.h"

// You can remove or add any includes you need
#include <memory>

#include "TargetSolver.h"

// Zadanie 2 to zadanie 4, tylko po prostu kolejka z 1 elementem
// dlatego zacznę od 4

int solver(const std::shared_ptr<backend_interface::Tester>& tester, bool preempt) {
  TargetSolver targetSolver(tester, preempt);
  targetSolver.run();
  return 0;
}

