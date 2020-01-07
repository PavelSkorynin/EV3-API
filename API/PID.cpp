/*
 * PD.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "PID.h"

namespace ev3 {

// don't update too often; 14375Hz = frequency of UART on PORT 1/2
const float UPDATE_STEP = 1.0f / 14375;

PID::PID(float kp, float ki, float kd, std::function<bool()> isCompleted)
	: kp(kp)
	, ki(ki)
	, kd(kd)
	, lastError(0.0f)
	, lastIntegralPart(0.0f)
	, lastUpdateTime(0.0f)
	, power(0.0f)
	, isCompletedFunc(isCompleted)
	, powerWire([this] { return this->power; })
{
}

PID::~PID() {
}

void PID::setError(const WireF & errorWire) {
	this->errorWire = std::make_shared<WireF>(errorWire);
}

float PID::getPower() const {
	return powerWire.getValue();
}

WireF PID::getPowerWire() const {
	return powerWire;
}

void PID::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	if (!errorWire) {
		return;
	}
	float error = errorWire->getValue();

	float dErr = (error - lastError);

	power = error * kp + lastIntegralPart + error * ki + dErr * kd;
	if (secondsFromStart - lastUpdateTime >= UPDATE_STEP) {
		lastUpdateTime = secondsFromStart;
		lastError = error;
		lastIntegralPart += error * ki;
	}
}

bool PID::isCompleted() const {
	return isCompletedFunc();
}

void PID::setPID(float kP, float kI, float kD) {
	this->kp = kP;
	this->ki = kI;
	this->kd = kD;
}


} /* namespace ev3 */
