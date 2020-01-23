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
	, minPower(7)
	, anchorEncoder(50)
	, powerThreshold(5)
	, speedThreshold(3)
	, powerPD(0.5f, 0.0f, 1.2f)
	, moveOnLineProcess(std::make_shared<MoveOnLineProcess>(leftMotor_, rightMotor_, leftLight_, rightLight_, INT32_MAX / 16, maxPower_)) {
}

void StopOnLineProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	powerPD.setError(WireF(encoderDistance) - (WireF(leftMotor->getEncoderWire()) + WireF(leftMotor->getEncoderWire())) / 2.0f);
}


void StopOnLineProcess::update(float secondsFromStart) {
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

	moveOnLineProcess->setMaxPower(relativePower);
	moveOnLineProcess->update(secondsFromStart);
}

bool StopOnLineProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	return powerPD.getPower() < powerThreshold && abs(leftMotor->getActualSpeed()) < speedThreshold && abs(rightMotor->getActualSpeed()) < speedThreshold;
}

void StopOnLineProcess::onCompleted(float secondsFromStart) {
	moveOnLineProcess->onCompleted(secondsFromStart);
	leftMotor->setPower(0);
	rightMotor->setPower(0);
}

void StopOnLineProcess::setMovePID(float kP, float kI, float kD) {
	moveOnLineProcess->setPID(kP, kI, kD);
}

void StopOnLineProcess::setPowerPID(float kP, float kI, float kD) {
	powerPD.setPID(kP, kI, kD);
}

void StopOnLineProcess::setMinPower(int minPower) {
	this->minPower = minPower;
}

void StopOnLineProcess::setAnchorEncoder(float anchorEncoder) {
	this->anchorEncoder = anchorEncoder;
}

void StopOnLineProcess::setPowerThreshold(int powerThreshold) {
	this->powerThreshold = powerThreshold;
}

void StopOnLineProcess::setSpeedThreshold(int speedThreshold) {
	this->speedThreshold = speedThreshold;
}

}
