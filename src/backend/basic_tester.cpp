#include "basic_tester.hpp"
#include "solver.hpp"

backend::BasicTester::BasicTester() {
  
}

std::shared_ptr<backend_interface::Component<uint8_t, uint16_t>>
  backend::BasicTester::get_motor_1() {

}

std::shared_ptr<backend_interface::Component<uint8_t, uint16_t>>
  backend::BasicTester::get_motor_2() {

}

std::shared_ptr<backend_interface::Component<std::pair<double, double>, backend_interface::Tester::Impossible>>
  backend::BasicTester::get_commands() {

}
