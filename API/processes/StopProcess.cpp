/*
 * StopProcess.cpp
 *
 *  Created on: 6 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "StopProcess.hpp"

namespace ev3 {

StopProcess::StopProcess(const MotorPtr &motor)
	: motor(motor)
	, speedThreshold(3) {
}

void StopProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	motor->setPower(0);
}

bool StopProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	return abs(motor->getActualSpeed()) < speedThreshold;
}

void StopProcess::setSpeedThreshold(int speedThreshold) {
	this->speedThreshold = speedThreshold;
}

} /* namespace ev3 */
