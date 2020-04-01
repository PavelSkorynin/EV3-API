/*
 * StopByEncoderOnArc.cpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "StopByEncoderOnArcProcess.hpp"

#include <EV3Math.h>
#include <cmath>

#include <core/ev3_lcd.h>

namespace ev3 {

StopByEncoderOnArcProcess::StopByEncoderOnArcProcess(MotorPtr leftMotor_, MotorPtr rightMotor_,
		int leftEncoderDistance_, int rightEncoderDistance_, int maxPower_)
	: leftMotor(leftMotor_)
	, rightMotor(rightMotor_)
	, leftEncoderDistance(leftEncoderDistance_ * (maxPower_ < 0 ? -1 : 1))
	, rightEncoderDistance(rightEncoderDistance_ * (maxPower_ < 0 ? -1 : 1))
	, maxPower(abs(maxPower_))
	, minPower(7)
	, dirLeft(leftEncoderDistance < 0 ? -1 : 1)
	, dirRight(rightEncoderDistance < 0 ? -1 : 1)
	, anchorEncoder(100)
	, powerThreshold(5)
	, speedThreshold(3)
	, powerPD(0.5f, 0.0f, 1.2f)
	, moveByEncoderOnArcProcess(std::make_shared<MoveByEncoderOnArcProcess>(leftMotor_, rightMotor_, leftEncoderDistance_, rightEncoderDistance_, maxPower_)) {
}

void StopByEncoderOnArcProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	leftEncoderStart = leftMotor->getEncoder();
	rightEncoderStart = rightMotor->getEncoder();

	powerPD.setError(WireF((leftEncoderDistance + leftEncoderStart) * dirLeft) + WireF((rightEncoderDistance + rightEncoderStart) * dirRight) - WireF(leftMotor->getEncoderWire() * dirLeft) - WireF(rightMotor->getEncoderWire() * dirRight));
}

void StopByEncoderOnArcProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	const float scale = 1.0f;

	powerPD.update(secondsFromStart);
	const float value = powerPD.getPower() / anchorEncoder;
	float relativePower = 0;
	relativePower = clamp<int>(powf(fabsf(value), 0.8f) * anchorEncoder * scale + minPower, -maxPower, maxPower);

	moveByEncoderOnArcProcess->setMaxPower(relativePower);
	moveByEncoderOnArcProcess->update(secondsFromStart);
}

bool StopByEncoderOnArcProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	auto power = (leftEncoderDistance + leftEncoderStart - leftMotor->getEncoder()) * dirLeft + (rightEncoderDistance + rightEncoderStart - rightMotor->getEncoder()) * dirRight;
	return (abs(power) < powerThreshold && abs(leftMotor->getActualSpeed()) < speedThreshold && abs(rightMotor->getActualSpeed()) < speedThreshold)
			|| moveByEncoderOnArcProcess->isCompleted(secondsFromStart);
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
