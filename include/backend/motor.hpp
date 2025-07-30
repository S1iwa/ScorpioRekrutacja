#pragma once

#include <cstdint>
#include <random>
#include "component.hpp"
#include "spinnable.hpp"

namespace backend {
class Motor final : public backend_interface::Component<uint8_t, uint16_t>, public Spinnable {
  double _cs;
  uint16_t _pose;
  std::mt19937 _random_generator;
  std::normal_distribution<double> _normal_distribution;
  std::function<void(const uint16_t&)> _data_callback;

public:
  explicit Motor(double control_signal, uint16_t pose);

  void spin(double) override;
  void add_data_callback(std::function<void(const uint16_t&)>) override;
  inline void send_data(const uint8_t& control_signal) override {
    _cs = control_signal;
  }
};
}  // namespace backend
