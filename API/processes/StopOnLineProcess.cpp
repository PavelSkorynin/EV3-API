/*
 * StopOnLineProcess.cpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "StopOnLineProcess.hpp"

#include <EV3Math.h>
#include <cmath>

namespace ev3 {

StopOnLineProcess::StopOnLineProcess(MotorPtr leftMotor_, MotorPtr rightMotor_,
		SensorPtr leftLight_, SensorPtr rightLight_,
		int encoderDistance_, int maxPower_)
	: leftMotor(leftMotor_)
	, rightMotor(rightMotor_)
	, leftLight(leftLight_)
	, rightLight(rightLight_)
	, encoderDistance(encoderDistance_)
	, maxPower(maxPower_)
	, powerPD(0.5f, 0.0f, 1.2f)
	, moveOnLineProcess(std::make_shared<MoveOnLineProcess>(leftMotor_, rightMotor_, leftLight_, rightLight_, INT32_MAX / 16, maxPower_)) {
}

void StopOnLineProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	powerPD.setError(WireF(encoderDistance) - (WireF(leftMotor->getEncoderWire()) + WireF(leftMotor->getEncoderWire())) / 2.0f);
}


void StopOnLineProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
    const int minPower = 7;
	const float anchor = 50;
	const float scale = 1.0f;

	powerPD.update(secondsFromStart);
	const float value = powerPD.getPower() / anchor;
	float relativePower = 0;
	if (value > 0) {
		relativePower = clamp<int>(powf(value, 0.8f) * anchor * scale + minPower, -abs(maxPower), abs(maxPower));
	} else {
		relativePower = clamp<int>(-powf(-value, 0.8f) * anchor * scale - minPower, -abs(maxPower), abs(maxPower));
	}

	moveOnLineProcess->setMaxPower(relativePower);
	moveOnLineProcess->update(secondsFromStart);
}

bool StopOnLineProcess::isCompleted() const {
	return powerPD.getPower() < 5 && (abs(leftMotor->getActualSpeed()) + abs(rightMotor->getActualSpeed())) < 4;
}

void StopOnLineProcess::onCompleted(float secondsFromStart) {
	moveOnLineProcess->onCompleted(secondsFromStart);
	leftMotor->setPower(0);
	rightMotor->setPower(0);
}

}
