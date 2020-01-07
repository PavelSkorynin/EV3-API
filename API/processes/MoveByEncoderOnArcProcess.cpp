/*
 * MoveByEncoderOnArcProcess.cpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "MoveByEncoderOnArcProcess.hpp"

#include <EV3Math.h>

namespace ev3 {

MoveByEncoderOnArcProcess::MoveByEncoderOnArcProcess(MotorPtr leftMotor_, MotorPtr rightMotor_, int leftEncoderDistance_, int rightEncoderDistance_, int maxPower_)
	: leftMotor(leftMotor_)
	, rightMotor(rightMotor_)
	, leftEncoderDistance(leftEncoderDistance_)
	, rightEncoderDistance(rightEncoderDistance_)
	, maxPower(maxPower_)
	, pd(0.4f, 0, 1.2f)
	, toggleLeftMotor(leftEncoderDistance_ < 0)
	, toggleRightMotor(rightEncoderDistance_ < 0) {
}

void MoveByEncoderOnArcProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	if (toggleLeftMotor) {
		leftMotor->toggleDirection();
		leftEncoderDistance = -leftEncoderDistance;
	}
	if (toggleRightMotor) {
		rightMotor->toggleDirection();
		rightEncoderDistance = -rightEncoderDistance;
	}
	leftMotor->resetEncoder();
	rightMotor->resetEncoder();

	float scaleLeft = 1, scaleRight = 1;

	if (leftEncoderDistance < rightEncoderDistance) {
		scaleLeft = (float)leftEncoderDistance / rightEncoderDistance;
	} else {
		scaleRight = (float)rightEncoderDistance / leftEncoderDistance;
	}

	// устанавливаем правило для вычисления ошибки в ПД-регуляторе
	// пересчёт значений будет происходить автоматически при обновлении ПД
	pd.setError(WireF(leftMotor->getEncoderWire()) * scaleRight - WireF(rightMotor->getEncoderWire()) * scaleLeft);

	if (scaleLeft * scaleRight > 0.5f) {
		// если оба мотора должны пройти примерно одинаковое расстояние
		leftMotor->setPower(WireI([&, scaleLeft, scaleRight] () -> int {
			if (pd.getPower() > 0) {
				auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
				return (maxPower - delta) * scaleLeft;
			}
			return maxPower * scaleLeft;
		}));
		rightMotor->setPower(WireI([&, scaleLeft, scaleRight] () -> int {
			if (pd.getPower() < 0) {
				auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
				return (maxPower + delta) * scaleRight;
			}
			return maxPower * scaleRight;
		}));
	} else {
		// если один мотор должен пройти существенно больше другого
		// первый мотор всегда вращается с максимальной скоростью, второй мотор становится ведомым
		leftMotor->setPower(WireI([&, scaleLeft, scaleRight] () -> int {
			if (leftEncoderDistance < rightEncoderDistance) {
				return clamp<float>(-pd.getPower() * maxPower / 50.0f, -abs(maxPower), abs(maxPower));
			} else {
				return maxPower;
			}
		}));
		rightMotor->setPower(WireI([&, scaleLeft, scaleRight] () -> int {
			if (leftEncoderDistance < rightEncoderDistance) {
				return maxPower;
			} else {
				return clamp<float>(pd.getPower() * maxPower / 50.0f, -abs(maxPower), abs(maxPower));
			}
		}));
	}
}

void MoveByEncoderOnArcProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	pd.update(secondsFromStart);
}

bool MoveByEncoderOnArcProcess::isCompleted() const {
	if (leftEncoderDistance > rightEncoderDistance) {
		return leftMotor->getEncoder() >= leftEncoderDistance;
	}
	if (rightEncoderDistance > leftEncoderDistance) {
		return rightMotor->getEncoder() >= rightEncoderDistance;
	}
	return leftMotor->getEncoder() >= leftEncoderDistance && rightMotor->getEncoder() >= rightEncoderDistance;
}

void MoveByEncoderOnArcProcess::onCompleted(float secondsFromStart) {
	leftMotor->setPower(leftMotor->getPower());
	rightMotor->setPower(rightMotor->getPower());

	if (toggleLeftMotor) {
		leftMotor->toggleDirection();
	}
	if (toggleRightMotor) {
		rightMotor->toggleDirection();
	}
}

void MoveByEncoderOnArcProcess::setMaxPower(int _maxPower) {
	maxPower = _maxPower;
}

void MoveByEncoderOnArcProcess::setPID(float kP, float kI, float kD) {
	pd.setPID(kP, kI, kD);
}


}
