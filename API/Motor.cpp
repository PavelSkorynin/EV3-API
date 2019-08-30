/*
 * Motor.cpp
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#include "Motor.h"

#include "core/ev3_output.h"
#include "core/ev3_lcd.h"

#include <math.h>

namespace ev3 {

template<typename T>
inline uint8_t P(const T &value) {
	return static_cast<uint8_t>(value);
}

Motor::Motor(Port port)
	: port(port)
	, direction(Direction::FORWARD)
	, maxAcceleration(2500.0f)
	, maxSpeed(70.0f) // ~20% of battery
	, lastSpeed(0)
	, lastTimestamp(0)
	, zeroEncoder(0)
	, speedInput([this]() -> int { return this->direction == Direction::FORWARD ? this->actualSpeed : -this->actualSpeed; })
	, encoderInput([this]() -> int {
		return this->direction == Direction::FORWARD
			? (this->encoder - this->zeroEncoder)
					: (this->zeroEncoder - this->encoder);
		})
	, tachoInput([this]() -> int { return this->tacho; })
{
	OnEx(P(port), RESET_NONE);
	Fwd(P(port));
	updateInputs(0);
	resetEncoder();
}

Motor::~Motor() {
	OffEx(P(port), RESET_NONE);
}

void Motor::setDirection(const Direction & direction) {
	if (this->direction == direction) {
		return;
	}

	this->direction = direction;
	SetDirection(P(port), P(direction));
}

WireI Motor::getActualSpeed() const {
	return speedInput;
}

WireI Motor::getEncoder() const {
	return encoderInput;
}

WireI Motor::getTacho() const {
	return encoderInput;
}

void Motor::setPower(const WireI & output) {
	powerOutput = std::make_shared<WireI>(output);
}

void Motor::resetEncoder() {
	zeroEncoder = encoder;
}

void Motor::setMaxAccelleration(float maxAcceleration) {
	this->maxAcceleration = maxAcceleration;
}

void Motor::updateInputs(float timestampSeconds) {
	// tacho or sensor ??
	OutputRead(P(port), &actualSpeed, &tacho, &encoder);
	if (abs(actualSpeed) > maxSpeed) {
		maxSpeed = abs(actualSpeed);
	}
	if (actualSpeed != lastSpeed) {
		lastSpeed = actualSpeed;
		lastTimestamp = timestampSeconds;
	}
}

void Motor::updateOutputs(float timestampSeconds) {
	if (!powerOutput) {
		return;
	}
	int targetPower = powerOutput->getValue();
	int currentSpeed = direction == Direction::FORWARD ? actualSpeed : -actualSpeed;;

	float dSpeed = targetPower * maxSpeed / 100.0f - currentSpeed;
	float delta = timestampSeconds - lastTimestamp;
	// too small step
	if (delta < 10e-9) {
		return;
	}
	float acceleration = maxAcceleration * powf(1.07f, fabsf(currentSpeed));
	if (fabsf(dSpeed) / delta > acceleration) {
		int sign = dSpeed >= 0 ? 1 : -1;
		targetPower = (int)(currentSpeed * 100.0f / maxSpeed + sign * delta * acceleration);
	}
	if (!OutputPower(P(port), targetPower)) {
		LcdTextf(0, 0, 60, "Output power error %d", P(port));
	}
}

} /* namespace ev3 */
