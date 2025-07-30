#include "motor.hpp"
#include <cmath>
#include <utility>

backend::Motor::Motor(double control_signal, uint16_t pose)
: _cs(control_signal), _pose(pose),
  _random_generator(std::random_device{}()), _normal_distribution(0.0, 1.0) { }

void backend::Motor::spin(double seconds) {
  constexpr double speed_factor = 0.006;
  _pose += static_cast<uint16_t>(_cs * _cs * seconds * speed_factor);
  if (_data_callback) {
    _data_callback(_pose + static_cast<uint16_t>(std::round(
      _normal_distribution(_random_generator)
    )));
  }
}

void backend::Motor::add_data_callback(std::function<void(const uint16_t&)> callback) {
  _data_callback = std::move(_data_callback);
}
