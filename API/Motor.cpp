/*
 * Motor.cpp
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#include "Motor.h"

#include "core/ev3_output.h"
#include "core/ev3_lcd.h"

namespace ev3 {

template<typename T>
inline uint8_t P(const T &value) {
	return static_cast<uint8_t>(value);
}

Motor::Motor(Port port)
	: port(port)
	, direction(Direction::FORWARD)
	, zeroEncoder(0)
	, speedInput([this]() -> int { return this->actualSpeed; })
	, encoderInput([this]() -> int {
		return this->direction == Direction::FORWARD
			? (this->encoder - this->zeroEncoder)
					: (this->zeroEncoder - this->encoder);
		})
{
	OnEx(P(port), RESET_NONE);
	Fwd(P(port));
	updateInputs();
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

void Motor::setPower(const WireI & output) {
	speedOutput.reset();
	powerOutput = std::make_shared<WireI>(output);
	updateOutputs();
}

void Motor::setSpeed(const WireI & output) {
	powerOutput.reset();
	speedOutput = std::make_shared<WireI>(output);
	updateOutputs();
}

void Motor::resetEncoder() {
	zeroEncoder = encoder;
}

void Motor::updateInputs() {
	int tacho;

	// tacho or sensor ??
	OutputRead(P(port), &actualSpeed, &tacho, &encoder);
}

void Motor::updateOutputs() {
	if (powerOutput) {
		if (!OutputPower(P(port), powerOutput->getValue())) {
			LcdTextf(1, 0, 60, "Output error %d", P(port));
		}
	}
	else if (speedOutput) {
		if (!OutputSpeed(P(port), speedOutput->getValue())) {
			LcdTextf(1, 0, 60, "Output error %d", P(port));
		}
	}
}


} /* namespace ev3 */
