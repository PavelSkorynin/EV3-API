/*
 * Motor.cpp
 *
 *  Created on: 09 May 2019
 *      Author: Pavel Skorynin
 */

#include "Motor.h"
#include "EV3Math.h"

#include "core/ev3_output.h"
#include "core/ev3_lcd.h"

#include <cmath>

namespace ev3 {

template<typename T>
inline uint8_t P(const T &value) {
	return static_cast<uint8_t>(value);
}

Motor::Motor(Port port)
	: port(port)
	, speedInput([this]() -> int { return this->direction == Direction::FORWARD ? this->actualSpeed : -this->actualSpeed; })
	, encoderInput([this]() -> int {
		return round(encoderScale * (this->direction == Direction::FORWARD
			? (this->encoder - this->zeroEncoder)
					: (this->zeroEncoder - this->encoder)));
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

void Motor::toggleDirection() {
	if (getDirection() == Motor::Direction::FORWARD) {
		setDirection(Motor::Direction::BACKWARD);
	} else {
		setDirection(Motor::Direction::FORWARD);
	}
}

int Motor::getActualSpeed() const {
	return speedInput.getValue();
}

WireI Motor::getActualSpeedWire() const {
	return speedInput;
}

int Motor::getEncoder() const {
	return encoderInput.getValue();
}

WireI Motor::getEncoderWire() const {
	return encoderInput;
}

int Motor::getTacho() const {
	return encoderInput.getValue();
}

WireI Motor::getTachoWire() const {
	return encoderInput;
}

void Motor::setPower(int power) {
	powerOutput = std::make_shared<WireI>(power);
}

void Motor::setPower(const WireI & output) {
	powerOutput = std::make_shared<WireI>(output);
}

float Motor::getActualPower() const {
	return (int)roundf(finePower);
}

int Motor::getPower() const {
	return powerOutput->getValue();
}

void Motor::resetEncoder() {
	zeroEncoder = encoder;
}

void Motor::blockOnEncoder(int targetEncoder) {
	setPower(WireI([&, targetEncoder] () -> int {
		return clamp<int>(targetEncoder - encoder, -100, 100);
	}));
}


void Motor::setMaxAccelleration(float maxAcceleration) {
	this->maxAcceleration = maxAcceleration;
}

void Motor::setEncoderScale(float scale) {
	this->encoderScale = scale;
}

void Motor::setSpeedOnMaxPower(float degreesPerSecond) {
	powerToSpeedRatio = degreesPerSecond / 100;
}

void Motor::setStartUpPower(int startUpPower) {
	this->startUpPower = startUpPower;
}



void Motor::updateInputs(float timestampSeconds) {
	// tacho or sensor ??
	OutputRead(P(port), &actualSpeed, &tacho, &encoder);
}

void Motor::updateOutputs(float timestampSeconds) {
	if (!powerOutput) {
		return;
	}
	if ((port == Port::C || port == Port::D) && MotorBusy(P(port))) {
		return;
	}
	float dt = timestampSeconds - prevTimestamp;
	prevTimestamp = timestampSeconds;

	float targetPower = powerOutput->getValue();

	if (fabs((targetPower - finePower) * powerToSpeedRatio) > maxAcceleration * dt) {
		if (targetPower > finePower) {
			finePower += maxAcceleration * dt / powerToSpeedRatio;
		} else {
			finePower -= maxAcceleration * dt / powerToSpeedRatio;
		}
	} else {
		finePower = targetPower;
	}

	int outPower = roundf(finePower);
	if (std::abs(targetPower) > 1 && std::abs(outPower) < startUpPower) {
		if (outPower > 0) {
			outPower = startUpPower;
		} else {
			outPower = -startUpPower;
		}
	}

	if (!OutputPower(P(port), outPower)) {
		LcdTextf(0, 0, 60, "Output power error %d", P(port));
	}
}

bool Motor::isBusy() {
	return MotorBusy(P(port));
}

} /* namespace ev3 */
