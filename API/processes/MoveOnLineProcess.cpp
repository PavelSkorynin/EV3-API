/*
 * MoveOnLineProcess.cpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#include "MoveOnLineProcess.hpp"
#include <EV3Math.h>

namespace ev3 {

MoveOnLineProcess::MoveOnLineProcess(MotorPtr leftMotor,
		MotorPtr rightMotor, SensorPtr leftLight,
		SensorPtr rightLight, int encoderDistance, int maxPower)
	: leftMotor(leftMotor)
	, rightMotor(rightMotor)
	, leftLight(leftLight)
	, rightLight(rightLight)
	, encoderDistance(encoderDistance)
	, maxPower(maxPower)
	, pd(0.3f, 0, 0.9f) {
}

void MoveOnLineProcess::onStarted(float secondsFromStart) {
	Process::onStarted(secondsFromStart);

	leftMotor->resetEncoder();
	rightMotor->resetEncoder();

	// устанавливаем правило для вычисления ошибки в ПД-регуляторе
	// пересчёт значений будет происходить автоматически при обновлении ПД
	pd.setError(rightLight->getValueWire() - leftLight->getValueWire());

	// устанавливаем правила для обновления скорости моторов
	// пересчёт значений будет происходить автоматически при обновлении выходов в EV3
	leftMotor->setPower(WireI([&] () -> int {
		auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
		if (delta > 0) {
			return maxPower - delta;
		}
		return maxPower;
	}));
	rightMotor->setPower(WireI([&] () -> int {
		auto delta = clamp<float>(maxPower * pd.getPower() / 50.0f, -abs(maxPower), abs(maxPower));
		if (delta < 0) {
			return maxPower + delta;
		}
		return maxPower;
	}));
}

void MoveOnLineProcess::update(float secondsFromStart) {
	Process::update(secondsFromStart);
	pd.update(secondsFromStart);
}

bool MoveOnLineProcess::isCompleted(float secondsFromStart) {
	Process::isCompleted(secondsFromStart);
	// останавливаемся, когда среднее значение на енкодерах достигло желаемого
	return (leftMotor->getEncoder() + rightMotor->getEncoder()) >= encoderDistance * 2;
}

void MoveOnLineProcess::onCompleted(float secondsFromStart) {
	leftMotor->setPower(leftMotor->getPower());
	rightMotor->setPower(rightMotor->getPower());
}

void MoveOnLineProcess::setMaxPower(int _maxPower) {
	maxPower = _maxPower;
}

void MoveOnLineProcess::setPID(float kP, float kI, float kD) {
	pd.setPID(kP, kI, kD);
}


}
