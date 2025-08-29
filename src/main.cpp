#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <type_traits>

#include "basic_tester.hpp"
#include "solver.hpp"

template<typename T>
void run_tests(T&& input_stream, double encoders_frequency) {
  static_assert(std::is_base_of_v<std::istream, std::decay_t<T>>,
    "Input type must be derived from std::istream");
  std::string line;
  std::vector<std::pair<double, Point>> signals;
  while (std::getline(input_stream, line)) {
    if (line.empty()) {
      continue;
    }
    std::istringstream line_stream(line);
    double delay;
    Point point;
    line_stream >> delay >> point.x >> point.y >> point.z;
    if (line_stream.fail() || !line_stream.eof()) {
      std::cerr << "Invalid input format\n";
      return;
    }
    if (delay < 0.0) {
      std::cerr << "Delay must be non-negative\n";
      return;
    }
    signals.emplace_back(delay, point);
  }
  auto tester = std::make_shared<backend::BasicTester>(encoders_frequency, signals);
  solver(tester);
}

void print_help(const char* const self) {
  std::cout << "Usage: " << self << " [options]\n"
    "Options:\n"
    "  -h        Show this help message\n"
    "  -f FILE   Read input from FILE instead of standard input\n"
    "  -g LIMIT  Enable debug output\n"
    "  -t LIMIT  Maximum (limit) encoder reading when moving vertical motor up\n"
    "  -d LIMIT  Minumum (limit) encoder reading when moving vertical motor down\n"
    "  -l LIMIT  Maximum (limit) encoder reading when moving horizontal motor right\n"
    "  -r LIMIT  Minumum (limit) encoder reading when moving horizontal motor left\n"
    "            Limits are in range [0, 4095]\n"
    "  -q FREQ   Encoders update frequency\n"
    ;
}

std::optional<const char*> get_next_arg(int& i, const int argc, const char* const argv[]) {
  ++i;
  if (i >= argc) {
    return std::nullopt;
  }
  return argv[i];
}

int main(const int argc, const char* const argv[]) {
  if (argc < 1) {
    std::cerr << "Invalid arguments\n";
    return 1;
  }
  std::optional<const char*> input_file;
  backend::Constraints constraints;
  double encoders_frequency = 0.05;
  int i = 1;
  auto get_next_arg = [&argc, &argv, &i]() -> std::optional<const char*> {
    ++i;
    if (i >= argc) {
      return std::nullopt;
    }
    return argv[i];
  };
  for (; i < argc; ++i) {
    if (std::strlen(argv[i]) != 2 || argv[i][0] != '-') {
      std::cerr << "Argument " << argv[i][0] << " is not valid write -h for help\n";
      return 1;
    }
    switch (argv[i][1]) {
      default: {
        std::cerr << "Argument " << argv[i][0] << " is not valid write -h for help\n";
        return 1;
      }
      case 'h': {
        print_help(argv[0]);
        return 0;
      }
      case 'f': {
        auto arg = get_next_arg();
        if (!arg.has_value()) {
          std::cerr << "Option -f requires a file name\n";
          return 1;
        }
        if (!std::filesystem::exists(argv[i])) {
          std::cerr << "File " << argv[i] << " does not exist\n";
          return 1;
        }
        input_file = argv[i];
      }
      case 't': {
        auto arg = get_next_arg();
        if (!arg.has_value()) {
          std::cerr << "Option -t requires a number\n";
          return 1;
        }
        try {
          auto v = std::stoul(*arg);
          if (v > 4095 || v < 0) {
            std::cerr << "Option -t requires a number in range [0, 4095]\n";
            return 1;
          }
          constraints.vertical_max = static_cast<uint16_t>(v);
        } catch (const std::exception&) {
          std::cerr << "Option -t requires a valid number\n";
          return 1;
        }
        break;
      }
      case 'd': {
        auto arg = get_next_arg();
        if (!arg.has_value()) {
          std::cerr << "Option -d requires a number\n";
          return 1;
        }
        try {
          auto v = std::stoul(*arg);
          if (v > 4095 || v < 0) {
            std::cerr << "Option -d requires a number in range [0, 4095]\n";
            return 1;
          }
          constraints.vertical_min = static_cast<uint16_t>(v);
        } catch (const std::exception&) {
          std::cerr << "Option -d requires a valid number\n";
          return 1;
        }
        break;
      }
      case 'l': {
        auto arg = get_next_arg();
        if (!arg.has_value()) {
          std::cerr << "Option -l requires a number\n";
          return 1;
        }
        try {
          auto v = std::stoul(*arg);
          if (v > 4095 || v < 0) {
            std::cerr << "Option -l requires a number in range [0, 4095]\n";
            return 1;
          }
          constraints.horizontal_min = static_cast<uint16_t>(v);
        } catch (const std::exception&) {
          std::cerr << "Option -l requires a valid number\n";
          return 1;
        }
        break;
      }
      case 'r': {
        auto arg = get_next_arg();
        if (!arg.has_value()) {
          std::cerr << "Option -r requires a number\n";
          return 1;
        }
        try {
          auto v = std::stoul(*arg);
          if (v > 4095 || v < 0) {
            std::cerr << "Option -r requires a number in range [0, 4095]\n";
            return 1;
          }
          constraints.horizontal_max = static_cast<uint16_t>(v);
        } catch (const std::exception&) {
          std::cerr << "Option -r requires a valid number\n";
          return 1;
        }
    }
    case 'q': {
        auto arg = get_next_arg();
        if (!arg.has_value()) {
          std::cerr << "Option -q requires a number\n";
          return 1;
        }
        try {
          encoders_frequency = std::stod(*arg);
          if (encoders_frequency <= 0.0) {
            std::cerr << "Option -q requires a positive number\n";
            return 1;
          }
        } catch (const std::exception&) {
          std::cerr << "Option -q requires a valid number\n";
          return 1;
        }
        break;
      }
    }
  }
  if (input_file.has_value()) {
    std::ifstream file_stream(*input_file);
    if (!file_stream.is_open()) {
      std::cerr << "Failed to open file " << *input_file << "\n";
      return 1;
    }
    run_tests(file_stream, encoders_frequency);
  } else {
    std::cout << "Reading from standard input, press Ctrl+D (or Ctrl+Z on Windows) to end input\n";
    run_tests(std::cin, encoders_frequency); 
  }
  return 0;
}
