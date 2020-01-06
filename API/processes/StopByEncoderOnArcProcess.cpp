/*
 * StopByEncoderOnArc.cpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "StopByEncoderOnArcProcess.hpp"

#include <EV3Math.h>
#include <cmath>

namespace ev3 {

StopByEncoderOnArcProcess::StopByEncoderOnArcProcess(MotorPtr leftMotor_, MotorPtr rightMotor_,
		int leftEncoderDistance_, int rightEncoderDistance_, int maxPower_)
	: leftMotor(leftMotor_)
	, rightMotor(rightMotor_)
	, leftEncoderDistance(leftEncoderDistance_)
	, rightEncoderDistance(rightEncoderDistance_)
	, maxPower(maxPower_)
	, powerPD(0.5f, 0.0f, 1.2f)
	, moveByEncoderOnArcProcess(std::make_shared<MoveByEncoderOnArcProcess>(leftMotor_, rightMotor_, leftEncoderDistance_ * 2, rightEncoderDistance_ * 2, maxPower_)) {
}

void StopByEncoderOnArcProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	powerPD.setError(WireF(abs(leftEncoderDistance) + abs(rightEncoderDistance)) - WireF(leftMotor->getEncoderWire()) - WireF(leftMotor->getEncoderWire()));
}

void StopByEncoderOnArcProcess::update(float secondsFromStart) {
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

	moveByEncoderOnArcProcess->setMaxPower(relativePower);
	moveByEncoderOnArcProcess->update(secondsFromStart);
}

bool StopByEncoderOnArcProcess::isCompleted() const {
	return powerPD.getPower() < 5 && (abs(leftMotor->getActualSpeed()) + abs(rightMotor->getActualSpeed())) < 4;
}

void StopByEncoderOnArcProcess::onCompleted(float secondsFromStart) {
	moveByEncoderOnArcProcess->onCompleted(secondsFromStart);
	leftMotor->setPower(0);
	rightMotor->setPower(0);
}

}
