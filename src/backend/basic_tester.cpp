#include <chrono>
#include <exception>
#include <iostream>
#include <utility>
#include "basic_tester.hpp"
#include "solver.hpp"

backend::BasicTester::BasicTester(double encoder_frequency, std::vector<std::pair<double, Point>> signals,
  Constraints constraints)
  : _motor1(std::make_shared<backend::Motor>(0, 0, constraints.vertical_max, constraints.vertical_min)),
    _motor2(std::make_shared<backend::Motor>(0, 0, constraints.horizontal_max, constraints.horizontal_min)),
    _commands(std::make_shared<backend::MockComponent<backend_interface::Tester::Impossible, Point>>()),
    _stop(false),
    _spinner(&BasicTester::spinner, this, encoder_frequency),
    _sender(&BasicTester::sender, this, std::move(signals)) { }

backend::BasicTester::~BasicTester() {
  _stop.store(true, std::memory_order_relaxed);
  _stop_cv.notify_all();
  _sender.join();
  _spinner.join();
}

void backend::BasicTester::spinner(double encoder_frequency) {
  const auto frame_duration = std::chrono::duration<double>(encoder_frequency);
  while (!_stop.load(std::memory_order_relaxed)) {
    const auto start = std::chrono::steady_clock::now();
    _motor1->spin(static_cast<double>(frame_duration.count()) / 1000.0);
    _motor2->spin(static_cast<double>(frame_duration.count()) / 1000.0);
    const auto end = std::chrono::steady_clock::now();
    const auto elapsed = end - start;
    if (elapsed > frame_duration) {
      std::cerr << "Terminating spinner - callbacks lasted for too long\n" << std::flush;
      std::terminate();
    }
    std::unique_lock<std::mutex> lock(_stop_mutex);
    _stop_cv.wait_for(lock, frame_duration - elapsed, [this]() { return _stop.load(std::memory_order_relaxed); });
  }
}

void backend::BasicTester::sender(std::vector<std::pair<double, Point>> signals) {
  for (size_t i = 0; !_stop.load(std::memory_order_relaxed) && i < signals.size(); ++i) {
    const auto [delay, point] = signals[i];
    std::unique_lock<std::mutex> lock(_stop_mutex);
    _stop_cv.wait_for(lock, std::chrono::duration<double>(delay),
      [this]() { return _stop.load(std::memory_order_relaxed); });
    if (_stop.load(std::memory_order_relaxed)) {
      break;
    }
    _commands->send_to_callback(point);
  }
}
