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
	, minPower(7)
	, anchorEncoder(50)
	, powerThreshold(5)
	, speedThreshold(3)
	, powerPD(0.5f, 0.0f, 1.2f)
	, moveByEncoderOnArcProcess(std::make_shared<MoveByEncoderOnArcProcess>(leftMotor_, rightMotor_, leftEncoderDistance_ * 2, rightEncoderDistance_ * 2, maxPower_)) {
}

void StopByEncoderOnArcProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	powerPD.setError(WireF(abs(leftEncoderDistance) + abs(rightEncoderDistance)) - WireF(leftMotor->getEncoderWire()) - WireF(leftMotor->getEncoderWire()));
}

void StopByEncoderOnArcProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	const float scale = 1.0f;

	powerPD.update(secondsFromStart);
	const float value = powerPD.getPower() / anchorEncoder;
	float relativePower = 0;
	if (value > 0) {
		relativePower = clamp<int>(powf(value, 0.8f) * anchorEncoder * scale + minPower, -abs(maxPower), abs(maxPower));
	} else {
		relativePower = clamp<int>(-powf(-value, 0.8f) * anchorEncoder * scale - minPower, -abs(maxPower), abs(maxPower));
	}

	moveByEncoderOnArcProcess->setMaxPower(relativePower);
	moveByEncoderOnArcProcess->update(secondsFromStart);
}

bool StopByEncoderOnArcProcess::isCompleted() const {
	return powerPD.getPower() < powerThreshold && abs(leftMotor->getActualSpeed()) < speedThreshold && abs(rightMotor->getActualSpeed()) < speedThreshold;
}

void StopByEncoderOnArcProcess::onCompleted(float secondsFromStart) {
	moveByEncoderOnArcProcess->onCompleted(secondsFromStart);
	leftMotor->setPower(0);
	rightMotor->setPower(0);
}

void StopByEncoderOnArcProcess::setMovePID(float kP, float kI, float kD) {
	moveByEncoderOnArcProcess->setPID(kP, kI, kD);
}

void StopByEncoderOnArcProcess::setPowerPID(float kP, float kI, float kD) {
	powerPD.setPID(kP, kI, kD);
}

void StopByEncoderOnArcProcess::setMinPower(int minPower) {
	this->minPower = minPower;
}

void StopByEncoderOnArcProcess::setAnchorEncoder(float anchorEncoder) {
	this->anchorEncoder = anchorEncoder;
}

void StopByEncoderOnArcProcess::setPowerThreshold(int powerThreshold) {
	this->powerThreshold = powerThreshold;
}

void StopByEncoderOnArcProcess::setSpeedThreshold(int speedThreshold) {
	this->speedThreshold = speedThreshold;
}


}
