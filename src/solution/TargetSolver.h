//
// Created by S1iwa on 31/10/2025.
//

#ifndef SCORPIO_RECRUITMENT_TASK_TARGETSOLVER_H
#define SCORPIO_RECRUITMENT_TASK_TARGETSOLVER_H

#include <atomic>
#include <mutex>
#include <queue>

#include "tester.hpp"
#include "component.hpp"

using backend_interface::Tester;
using backend_interface::Component;

struct targetAngles {
	double horizontalAngle;
	double verticalAngle;
};

class TargetSolver {
public:
	explicit TargetSolver(const std::shared_ptr<Tester>& tester, bool preempt);
	void run();
private:
	// Callback for new target points
	void onNewTarget(Point point);
	void onEncoder1Update(const uint16_t& value);
	void onEncoder2Update(const uint16_t& value);

	// Calculate angles for a given point
	static targetAngles calculateAngles(Point point);
	static double calcHorizontalAngle(Point point);
	static double calcVerticalAngle(Point point);

	bool preemptMode;
	std::shared_ptr<Component<int8_t, uint16_t>> motor1;
	std::shared_ptr<Component<int8_t, uint16_t>> motor2;

	// Queue for 4th task
	std::queue<Point> targetsQueue;
	std::mutex targetsMutex;

	// Current encoders values
	std::atomic<uint16_t> encoder1Value;
	std::atomic<uint16_t> encoder2Value;

	// Current processed target point
	std::atomic<bool> hasActiveTarget;
	targetAngles currentTargetAngles;
	std::mutex currentTargetMutex;
};

#endif //SCORPIO_RECRUITMENT_TASK_TARGETSOLVER_H