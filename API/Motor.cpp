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
{
	updateInputs();
	speedInput = std::make_shared<WireI>([this]() { return actualSpeed; });
	encoderInput = std::make_shared<WireI>([this]() { return encoder; });
}

Motor::~Motor() {
}

std::weak_ptr<WireI> Motor::getActualSpeed() const {
	return speedInput;
}

std::weak_ptr<WireI> Motor::getEncoder() const {
	return encoderInput;
}

void Motor::setPower(const std::weak_ptr<WireI> & output) {
	powerOutput = output;
}

void Motor::setSpeed(const std::weak_ptr<WireI> & output) {
	speedOutput = output;
}

void Motor::updateInputs() {
	int8_t speed;
	int tacho;
	int sensor;
	OutputRead(port, &speed, &tacho, &sensor);
	encoder = tacho;
	actualSpeed = speed;
}

void Motor::updateOutputs() {
	if (auto ptr = powerOutput.lock()) {
		if (!OutputPower(port, ptr->getValue())) {
			printf("Failed to set power for motor on port %d\n", port);
		}
	}
	else if (auto ptr = speedOutput.lock()) {
		if (!OutputSpeed(port, ptr->getValue())) {
			printf("Failed to set speed for motor on port %d\n", port);
		}
	}
}


} /* namespace ev3 */
