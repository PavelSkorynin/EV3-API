/*
 * MoveByEncoderOnArcProcess.cpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "MoveByEncoderOnArcProcess.hpp"

#include <EV3Math.h>
#include <cmath>

namespace ev3 {

MoveByEncoderOnArcProcess::MoveByEncoderOnArcProcess(MotorPtr leftMotor_, MotorPtr rightMotor_, int leftEncoderDistance_, int rightEncoderDistance_, int maxPower_)
	: leftMotor(leftMotor_)
	, rightMotor(rightMotor_)
	, leftEncoderDistance(leftEncoderDistance_ * (maxPower_ < 0 ? -1 : 1))
	, rightEncoderDistance(rightEncoderDistance_ * (maxPower_ < 0 ? -1 : 1))
	, maxPower(abs(maxPower_))
	, scaleLeft(abs(leftEncoderDistance) < abs(rightEncoderDistance) ? fabsf((float)leftEncoderDistance / rightEncoderDistance) : 1)
	, scaleRight(abs(leftEncoderDistance) < abs(rightEncoderDistance) ? 1 : fabsf((float)rightEncoderDistance / leftEncoderDistance))
	, dirLeft(leftEncoderDistance < 0 ? -1 : 1)
	, dirRight(rightEncoderDistance < 0 ? -1 : 1)
	, pd(0.4f, 0, 1.2f) {
}

void MoveByEncoderOnArcProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);
	leftEncoderStart = leftMotor->getEncoder();
	rightEncoderStart = rightMotor->getEncoder();

	// устанавливаем правило для вычисления ошибки в ПД-регуляторе
	// пересчёт значений будет происходить автоматически при обновлении ПД
	pd.setError(WireF(leftMotor->getEncoderWire() - WireI(leftEncoderStart)) * (scaleRight * dirLeft) - WireF(rightMotor->getEncoderWire() - WireI(rightEncoderStart)) * (scaleLeft * dirRight));
}

void MoveByEncoderOnArcProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	pd.update(secondsFromStart);

	if (scaleLeft * scaleRight > 0.5f) {
		// если оба мотора должны пройти примерно одинаковое расстояние
		int leftPower = maxPower;
		if (pd.getPower() > 0) {
			auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
			leftPower = (maxPower - delta);
		}

		int rightPower = maxPower;
		if (pd.getPower() < 0) {
			auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
			rightPower = (maxPower + delta);
		}
		leftMotor->setPower(leftPower * scaleLeft * dirLeft);
		rightMotor->setPower(rightPower * scaleRight * dirRight);
	} else {
		// если один мотор должен пройти существенно больше другого
		// первый мотор всегда вращается с максимальной скоростью, второй мотор становится ведомым
		int leftPower = maxPower * scaleLeft * dirLeft;
		if (abs(leftEncoderDistance) < abs(rightEncoderDistance)) {
			leftPower = clamp<float>(-pd.getPower() * this->maxPower / 50.0f, -abs(this->maxPower), abs(this->maxPower)) * dirLeft;
		}

		int rightPower = maxPower * scaleRight * dirRight;
		if (abs(rightEncoderDistance) < abs(leftEncoderDistance)) {
			rightPower = clamp<float>(pd.getPower() * this->maxPower / 50.0f, -abs(this->maxPower), abs(this->maxPower)) * dirRight;
		}
		leftMotor->setPower(leftPower);
		rightMotor->setPower(rightPower);
	}
}

bool MoveByEncoderOnArcProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	if (abs(leftEncoderDistance) > abs(rightEncoderDistance)) {
		return (leftMotor->getEncoder() - leftEncoderStart) * dirLeft >= leftEncoderDistance * dirLeft;
	}
	if (rightEncoderDistance > leftEncoderDistance) {
		return (rightMotor->getEncoder() - rightEncoderStart) * dirRight >= rightEncoderDistance * dirRight;
	}
	return (leftMotor->getEncoder() - leftEncoderStart) * dirLeft >= leftEncoderDistance * dirLeft && (rightMotor->getEncoder() - rightEncoderStart) * dirRight >= rightEncoderDistance * dirRight;
}

void MoveByEncoderOnArcProcess::onCompleted(float secondsFromStart) {
	leftMotor->setPower(leftMotor->getPower());
	rightMotor->setPower(rightMotor->getPower());
}

void MoveByEncoderOnArcProcess::setMaxPower(int _maxPower) {
	maxPower = _maxPower;
}

void MoveByEncoderOnArcProcess::setPID(float kP, float kI, float kD) {
	pd.setPID(kP, kI, kD);
}


}
