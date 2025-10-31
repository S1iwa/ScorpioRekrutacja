//
// Created by S1iwa on 31/10/2025.
//

#ifndef SCORPIO_RECRUITMENT_TASK_SOLUTION_H
#define SCORPIO_RECRUITMENT_TASK_SOLUTION_H

int solver(std::shared_ptr<backend_interface::Tester> tester, bool preempt);

double calcHorizontalAngle(Point point);
double calcVerticalAngle(Point point);

#endif //SCORPIO_RECRUITMENT_TASK_SOLUTION_H