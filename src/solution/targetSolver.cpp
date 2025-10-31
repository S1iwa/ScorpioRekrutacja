//
// Created by S1iwa on 31/10/2025.
//

#include "targetSolver.h"

#include <cmath>

targetAngles targetSolver::calculateAngles(Point point) {
	return targetAngles {
		calcHorizontalAngle(point),
		calcVerticalAngle(point),
	};
}

double targetSolver::calcHorizontalAngle(Point point) {
	return atan2(point.y, point.x) * 180.0 / M_PI;
}

double targetSolver::calcVerticalAngle(Point point) {
	double distance = sqrt(point.x * point.x + point.y * point.y);
	return atan2(point.z, distance) * 180.0 / M_PI;
}