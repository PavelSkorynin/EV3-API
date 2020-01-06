/*
 * StopProcess.cpp
 *
 *  Created on: 6 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "StopProcess.hpp"

namespace ev3 {

StopProcess::StopProcess(const MotorPtr &motor)
	: motor(motor) {
}

void StopProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	motor->setPower(0);
}

bool StopProcess::isCompleted() const {
	 return abs(motor->getActualSpeed()) < 3;
}

} /* namespace ev3 */
