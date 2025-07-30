#pragma once

#include "component.hpp"
#include "tester.hpp"

namespace backend {
class BasicTester final : public backend_interface::Tester {
public:
  BasicTester();

  std::shared_ptr<backend_interface::Component<uint8_t, uint16_t>> get_motor_1() override;
  std::shared_ptr<backend_interface::Component<uint8_t, uint16_t>> get_motor_2() override;
  std::shared_ptr<backend_interface::Component<std::pair<double, double>, Impossible>> get_commands() override;
};
}  // namespace backend
