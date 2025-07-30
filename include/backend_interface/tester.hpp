#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include "component.hpp"

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

  virtual std::shared_ptr<Component<uint8_t, uint16_t>> get_engine_1() = 0;
  virtual std::shared_ptr<Component<uint8_t, uint16_t>> get_engine_2() = 0;
  virtual std::shared_ptr<Component<std::pair<double, double>, Impossible>> get_commands() = 0;
};
}  // namespace backend_interface
