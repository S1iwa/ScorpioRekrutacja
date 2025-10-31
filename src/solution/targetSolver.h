//
// Created by S1iwa on 31/10/2025.
//

#ifndef SCORPIO_RECRUITMENT_TASK_TARGETSOLVER_H
#define SCORPIO_RECRUITMENT_TASK_TARGETSOLVER_H
#include <mutex>
#include <queue>

#include "tester.hpp"

struct targetAngles {
	double horizontalAngle;
	double verticalAngle;
};

class targetSolver {
public:
	targetSolver();
	void run();
private:
	void onNewTarget(Point point);
	static targetAngles calculateAngles(Point point) ;
	void sendToMotors(targetAngles angles);

	static double calcHorizontalAngle(Point point);
	static double calcVerticalAngle(Point point);

	std::queue<Point> targetsQueue;
	std::mutex targetsMutex;
};

#endif //SCORPIO_RECRUITMENT_TASK_TARGETSOLVER_H