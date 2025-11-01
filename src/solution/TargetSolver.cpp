//
// Created by S1iwa on 31/10/2025.
//

#include "TargetSolver.h"

#include <cmath>
#include <thread>

TargetSolver::TargetSolver(const std::shared_ptr<Tester>& tester, bool preempt) {
	// Save all components
	motor1 = tester->get_motor_1();
	motor2 = tester->get_motor_2();
	auto commands = tester->get_commands();

	// Register callbacks
	using namespace std::placeholders;
	commands->add_data_callback(std::bind(&TargetSolver::onNewTarget, this, _1));
	motor1->add_data_callback(std::bind(&TargetSolver::onEncoder1Update, this, _1));
	motor2->add_data_callback(std::bind(&TargetSolver::onEncoder2Update, this, _1));
}

void TargetSolver::run() {
	const double angleStep = 1.0; // Angle step for each motor command
	const double tolerance = 0.01; // Tolerance for angle comparison

	const double encoderToRadians = 2.0 * M_PI / 4095.0; // Conversion factor from encoder value to radians

	while (true) {
		if (!preemptMode && !hasActiveTarget.load()) {
			std::lock_guard lock(targetsMutex);
			if (!targetsQueue.empty()) {
				Point nextTarget = targetsQueue.front();
				targetsQueue.pop();

				std::lock_guard targetLock(currentTargetMutex);
				currentTargetAngles = calculateAngles(nextTarget);
				hasActiveTarget.store(true);
			}
		}

		if (hasActiveTarget.load()) {
			std::lock_guard targetLock(currentTargetMutex);

			double currentHorizontalAngle = encoder1Value.load() * encoderToRadians;
			double currentVerticalAngle = encoder2Value.load() * encoderToRadians;

			double errorHorizonstalAngle = currentTargetAngles.horizontalAngle - currentHorizontalAngle;
			double errorVerticalAngle = currentTargetAngles.verticalAngle - currentVerticalAngle;

			if (std::abs(errorHorizonstalAngle) < tolerance
				&& std::abs(errorVerticalAngle) < tolerance) {
				hasActiveTarget.store(false);
				motor1->send_data(0);
				motor2->send_data(0);
			} else {
				auto motor1Command = static_cast<int8_t>(std::clamp(angleStep * errorHorizonstalAngle, -127.0, 127.0));
				auto motor2Command = static_cast<int8_t>(std::clamp(angleStep * errorVerticalAngle, -127.0, 127.0));
				motor1->send_data(motor1Command);
				motor2->send_data(motor2Command);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

void TargetSolver::onNewTarget(const Point& point) {
	if (preemptMode) {
		std::lock_guard targetLock(currentTargetMutex);
		currentTargetAngles = calculateAngles(point);
		hasActiveTarget = true;
	} else {
		std::lock_guard lock(targetsMutex);
		targetsQueue.push(point);
	}
}

void TargetSolver::onEncoder1Update(const uint16_t& value) {
	encoder1Value.store(value);
}

void TargetSolver::onEncoder2Update(const uint16_t& value) {
	encoder2Value.store(value);
}

targetAngles TargetSolver::calculateAngles(Point point) {
	return targetAngles {
		calcHorizontalAngle(point),
		calcVerticalAngle(point),
	};
}

double TargetSolver::calcHorizontalAngle(Point point) {
	return atan2(point.y, point.x) * 180.0 / M_PI;
}

double TargetSolver::calcVerticalAngle(Point point) {
	double distance = sqrt(point.x * point.x + point.y * point.y);
	return atan2(point.z, distance) * 180.0 / M_PI;
}