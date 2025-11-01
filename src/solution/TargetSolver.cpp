//
// Created by S1iwa on 31/10/2025.
//

#include "TargetSolver.h"

#include <cmath>

TargetSolver::TargetSolver(const std::shared_ptr<backend_interface::Tester>& tester, bool preempt) {
	auto commands = tester->get_commands();
	commands->add_data_callback([this](const Point& point) {
		this->onNewTarget(point);
	});
}

void TargetSolver::run()
{
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