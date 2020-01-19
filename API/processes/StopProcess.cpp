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

/*
StopProcess::StopProcess(StopProcess&& other)
	: Process(std::move(other))
	, motor(std::move(other.motor))
	, speedThreshold(other.speedThreshold) {

}

StopProcess::StopProcess(const StopProcess& other)
	: Process(other)
	, motor(other.motor)
	, speedThreshold(other.speedThreshold) {

}

StopProcess& StopProcess::operator=(const StopProcess& other) {
	isStarted = other.isStarted;
	motor = other.motor;
	speedThreshold = other.speedThreshold;
	return *this;
}

StopProcess& StopProcess::operator=(StopProcess&& other) {
	isStarted = other.isStarted;
	motor = std::move(other.motor);
	speedThreshold = other.speedThreshold;
	return *this;
}
*/

void StopProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	motor->setPower(0);
}

bool StopProcess::isCompleted() const {
	 return abs(motor->getActualSpeed()) < speedThreshold;
}

void StopProcess::setSpeedThreshold(int speedThreshold) {
	this->speedThreshold = speedThreshold;
}

} /* namespace ev3 */
