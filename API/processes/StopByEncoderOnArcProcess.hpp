/*
 * StopByEncoderOnArc.hpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Motor.h> // для моторов
#include <Sensor.h> // для датчиков
#include <PID.h>
#include "MoveByEncoderOnArcProcess.hpp"

namespace ev3 {

class StopByEncoderOnArcProcess: public virtual Process {
public:
	StopByEncoderOnArcProcess(MotorPtr leftMotor_, MotorPtr rightMotor_,
			int leftEncoderDistance_, int rightEncoderDistance_, int maxPower_);

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted(float secondsFromStart) override;

	/**
	 * Установить коэффициенты ПИД-регулятора для движения по енкодеру
	 * @param kP пропорциональная составляющая
	 * @param kI интегральная составляющая
	 * @param kD дифференциальная составляющая
	 */
	void setMovePID(float kP, float kI, float kD);

	/**
	 * Установить коэффициенты ПИД-регулятора для максимальной мощности
	 * Значения по умолчанию: 0.5f, 0.0f, 1.2f
	 * @param kP пропорциональная составляющая
	 * @param kI интегральная составляющая
	 * @param kD дифференциальная составляющая
	 */
	void setPowerPID(float kP, float kI, float kD);

	/**
	 * Установить минимальное значение устанавливаемой скорости.
	 * Мощность на моторы будет подаваться не меньше этого значения
	 * @param minPower минимальная подаваемая мощность, значение по умолчанию 7
	 */
	void setMinPower(int minPower);

	/**
	 * Установить значение енкодера при котором начинается резкое замедление
	 * @param anchorEncoder переломное значение енкодера, значение по умолчанию 50
	 */
	void setAnchorEncoder(float anchorEncoder);

	/**
	 * Мощность, подаваемая на моторы, при которой завершается процесс
	 * @param powerThreshold минимальная мощность, значение по умолчанию 5
	 */
	void setPowerThreshold(int powerThreshold);

	/**
	 * Скорость на моторах (actual speed), при которой завершается процесс
	 * @param speedThreshold минимальная скорость, значение по умолчанию 3
	 */
	void setSpeedThreshold(int speedThreshold);

protected:
	MotorPtr leftMotor;
	MotorPtr rightMotor;
	int leftEncoderDistance;
	int rightEncoderDistance;
	int leftEncoderStart = 0;
	int rightEncoderStart = 0;
	int maxPower;
	int minPower;
	const int dirLeft;
	const int dirRight;
	float anchorEncoder;
	int powerThreshold;
	int speedThreshold;

	PID powerPD;
	std::shared_ptr<MoveByEncoderOnArcProcess> moveByEncoderOnArcProcess;
};

}
