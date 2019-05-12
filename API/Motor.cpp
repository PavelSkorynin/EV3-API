/*
 * Motor.cpp
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#include "Motor.h"

#include "core/ev3_output.h"

namespace ev3 {

Motor::Motor(Port port)
	: port(port)
	, direction(FORWARD)
	, zeroEncoder(0)
	, speedInput([this]() { return actualSpeed; })
	, encoderInput([this]() { return direction == FORWARD ? encoder - zeroEncoder : zeroEncoder - encoder; })
{
	OnEx(port, RESET_NONE);
	Fwd(port);
	updateInputs();
	resetEncoder();
}

Motor::~Motor() {
	OffEx(port, RESET_NONE);
}

void Motor::setDirection(const Direction & direction) {
	if (this->direction == direction) {
		return;
	}

	this->direction = direction;
	SetDirection(port, direction);
}

WireI Motor::getActualSpeed() const {
	return speedInput;
}

WireI Motor::getEncoder() const {
	return encoderInput;
}

void Motor::setPower(const WireI & output) {
	powerOutput = std::make_shared<WireI>(output);
	updateOutputs();
}

void Motor::setSpeed(const WireI & output) {
	speedOutput = std::make_shared<WireI>(output);
	updateOutputs();
}

void Motor::resetEncoder() {
	zeroEncoder = encoder;
}

void Motor::updateInputs() {
	int8_t speed;
	int tacho;
	int sensor;
	OutputRead(port, &speed, &tacho, &sensor);
	// tacho or sensor ??
	encoder = tacho;
	actualSpeed = speed;
}

void Motor::updateOutputs() {
	if (powerOutput) {
		if (!OutputPower(port, powerOutput->getValue())) {
			printf("Failed to set power for motor on port %d\n", port);
		}
	}
	else if (speedOutput) {
		if (!OutputSpeed(port, speedOutput->getValue())) {
			printf("Failed to set speed for motor on port %d\n", port);
		}
	}
}


} /* namespace ev3 */
