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
	, leftEncoderDistance(leftEncoderDistance_ * (maxPower_ < 0 ? -1 : 1))
	, rightEncoderDistance(rightEncoderDistance_ * (maxPower_ < 0 ? -1 : 1))
	, maxPower(abs(maxPower_))
	, pd(0.4f, 0, 1.2f)
	, toggleLeftMotor(leftEncoderDistance < 0)
	, toggleRightMotor(rightEncoderDistance < 0) {
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
	leftEncoderStart = leftMotor->getEncoder();
	rightEncoderStart = rightMotor->getEncoder();

	float scaleLeft = 1, scaleRight = 1;

	if (leftEncoderDistance < rightEncoderDistance) {
		scaleLeft = (float)leftEncoderDistance / rightEncoderDistance;
	} else {
		scaleRight = (float)rightEncoderDistance / leftEncoderDistance;
	}

	// устанавливаем правило для вычисления ошибки в ПД-регуляторе
	// пересчёт значений будет происходить автоматически при обновлении ПД
	pd.setError(WireF(leftMotor->getEncoderWire() - WireI(leftEncoderStart)) * scaleRight - WireF(rightMotor->getEncoderWire() - WireI(rightEncoderStart)) * scaleLeft);
}

void MoveByEncoderOnArcProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	pd.update(secondsFromStart);


	float scaleLeft = 1, scaleRight = 1;

	if (leftEncoderDistance < rightEncoderDistance) {
		scaleLeft = (float)leftEncoderDistance / rightEncoderDistance;
	} else {
		scaleRight = (float)rightEncoderDistance / leftEncoderDistance;
	}

	if (scaleLeft * scaleRight > 0.5f) {
		// если оба мотора должны пройти примерно одинаковое расстояние
		int leftPower = maxPower * scaleLeft;
		if (pd.getPower() > 0) {
			auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
			leftPower = (maxPower - delta) * scaleLeft;
		}

		int rightPower = maxPower * scaleRight;
		if (pd.getPower() < 0) {
			auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
			rightPower = (maxPower + delta) * scaleLeft;
		}
		leftMotor->setPower(leftPower);
		rightMotor->setPower(rightPower);
	} else {
		// если один мотор должен пройти существенно больше другого
		// первый мотор всегда вращается с максимальной скоростью, второй мотор становится ведомым
		int leftPower = maxPower;
		if (leftEncoderDistance < rightEncoderDistance) {
			leftPower = clamp<float>(-pd.getPower() * this->maxPower / 50.0f, -abs(this->maxPower), abs(this->maxPower));
		}

		int rightPower = maxPower;
		if (rightEncoderDistance < leftEncoderDistance) {
			rightPower = clamp<float>(pd.getPower() * this->maxPower / 50.0f, -abs(this->maxPower), abs(this->maxPower));
		}
		leftMotor->setPower(leftPower);
		rightMotor->setPower(rightPower);
	}
}

bool MoveByEncoderOnArcProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	if (leftEncoderDistance > rightEncoderDistance) {
		return leftMotor->getEncoder() >= leftEncoderStart + leftEncoderDistance;
	}
	if (rightEncoderDistance > leftEncoderDistance) {
		return rightMotor->getEncoder() >= rightEncoderStart + rightEncoderDistance;
	}
	return leftMotor->getEncoder() >= leftEncoderStart + leftEncoderDistance && rightMotor->getEncoder() >= rightEncoderStart + rightEncoderDistance;
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
