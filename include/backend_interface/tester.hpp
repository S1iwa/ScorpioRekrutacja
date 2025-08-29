#pragma once

#include <cstdint>
#include <memory>
#include "component.hpp"

struct Point {
  double x;
  double y;
  double z;
};

namespace backend_interface {
class Tester {
public:
  virtual ~Tester() = default;

  class Impossible final {
    Impossible() = delete;
    Impossible(const Impossible&) = delete;
    Impossible& operator=(const Impossible&) = delete;
    Impossible(Impossible&&) = delete;
    Impossible& operator=(Impossible&&) = delete;
  };

  virtual std::shared_ptr<Component<int8_t, uint16_t>> get_motor_1() = 0;
  virtual std::shared_ptr<Component<int8_t, uint16_t>> get_motor_2() = 0;
  virtual std::shared_ptr<Component<Impossible, Point>> get_commands() = 0;
};
}  // namespace backend_interface
