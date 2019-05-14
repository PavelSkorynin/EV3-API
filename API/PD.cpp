/*
 * PD.cpp
 *
 *  Created on: 11 May 2019
 *      Author: Pavel Skorynin
 */

#include "PD.h"

namespace ev3 {

// don't update too often; 14375Hz = frequency of UART on PORT 1/2
const float UPDATE_STEP = 1.0f / 14375;

PD::PD(float kp, float kd)
	: kp(kp)
	, kd(kd)
	, lastError(0.0f)
	, lastUpdateTime(0.0f)
	, power(0.0f)
	, powerWire([this] { return this->power; })
{
}

PD::~PD() {
}

void PD::setError(const WireF & errorWire) {
	this->errorWire = std::make_shared<WireF>(errorWire);
}

WireF PD::getPower() {
	return powerWire;
}

void PD::update(float secondsFromStart) {
	if (!errorWire) {
		return;
	}
	float error = errorWire->getValue();

	float dErr = (error - lastError);

	power = error * kp + dErr * kd;
	if (secondsFromStart - lastUpdateTime >= UPDATE_STEP) {
		lastUpdateTime = secondsFromStart;
		lastError = error;
	}
}

} /* namespace ev3 */
