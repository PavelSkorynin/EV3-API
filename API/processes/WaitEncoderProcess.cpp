/*
 * WaitEncoderProcess.cpp
 *
 *  Created on: 28 янв. 2020 г.
 *      Author: Pavel Skorynin
 */

#include "WaitEncoderProcess.hpp"

namespace ev3 {

WaitEncoderProcess::WaitEncoderProcess(MotorPtr motor, int targetEncoder)
	: motor(std::move(motor))
	, targetEncoder(targetEncoder) {
}

bool WaitEncoderProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	if ((motor->getDirection() == Motor::Direction::FORWARD && motor->getPower() >= 0)
			|| (motor->getDirection() == Motor::Direction::BACKWARD && motor->getPower() < 0)) {
		return motor->getEncoder() >= targetEncoder;
	} else {
		return motor->getEncoder() <= targetEncoder;
	}
}

} /* namespace ev3 */
