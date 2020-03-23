/*
 * SetPowerProcess.cpp
 *
 *  Created on: 20 февр. 2020 г.
 *      Author: Pavel
 */

#include "SetPowerProcess.hpp"

namespace ev3 {

SetPowerProcess::SetPowerProcess(MotorPtr motor, int power)
	: motor(std::move(motor))
	, power(power) {
}

void SetPowerProcess::onCompleted(float secondsFromStart) {
	Process::onCompleted(secondsFromStart);

	motor->setPower(power);
}

bool SetPowerProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);

	return true;
}

} /* namespace ev3 */
