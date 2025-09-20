#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <thread>
#include "component.hpp"
#include "mock_component.hpp"
#include "motor.hpp"
#include "tester.hpp"

namespace backend {

class BasicTester final : public backend_interface::Tester {
  std::shared_ptr<backend::Motor> _motor1;
  std::shared_ptr<backend::Motor> _motor2;
  std::shared_ptr<backend::MockComponent<Impossible, Point>> _commands;
  std::atomic<bool> _stop;
  std::mutex _stop_mutex;
  std::condition_variable _stop_cv;
  std::thread _spinner;
  std::thread _sender;

  void spinner(double encoder_period, bool debug);
  void sender(std::vector<std::pair<double, Point>> signals);

public:
  BasicTester(double encoder_period, std::vector<std::pair<double, Point>> signals,
    bool debug = false, Constraints constraints = {});
  ~BasicTester();

  inline std::shared_ptr<backend_interface::Component<int8_t, uint16_t>> get_motor_1() noexcept override {
    return std::static_pointer_cast<backend_interface::Component<int8_t, uint16_t>>(_motor1);
  }
  inline std::shared_ptr<backend_interface::Component<int8_t, uint16_t>> get_motor_2() noexcept override {
    return std::static_pointer_cast<backend_interface::Component<int8_t, uint16_t>>(_motor2);
  }
  inline std::shared_ptr<backend_interface::Component<Impossible, Point>> get_commands() noexcept override {
    return std::static_pointer_cast<backend_interface::Component<Impossible, Point>>(_commands);
  }
};
}  // namespace backend
