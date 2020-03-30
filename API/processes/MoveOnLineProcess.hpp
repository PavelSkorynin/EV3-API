/*
 * MoveOnLineProcess.hpp
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Motor.h> // для моторов
#include <Sensor.h> // для датчиков
#include <PID.h>

namespace ev3 {

class MoveOnLineProcess: public virtual Process {
public:
	MoveOnLineProcess(MotorPtr leftMotor, MotorPtr rightMotor,
			SensorPtr leftLight, SensorPtr rightLight,
			int encoderDistance, int maxPower = 70);

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted(float secondsFromStart) override;

	/**
	 * Установить максимальную мощность двигателей
	 * @param maxPower максимальная мощность (от 0 до 100)
	 */
	void setMaxPower(int maxPower);

	/**
	 * Установить коэффициенты ПИД-регулятора
	 * Значения по умолчанию: 0.3f, 0, 0.9f
	 * @param kP пропорциональная составляющая
	 * @param kI интегральная составляющая
	 * @param kD дифференциальная составляющая
	 */
	void setPID(float kP, float kI, float kD);

protected:
	MotorPtr leftMotor;
	MotorPtr rightMotor;
	SensorPtr leftLight;
	SensorPtr rightLight;
	int encoderDistance;
	int leftEncoderStart = 0;
	int rightEncoderStart = 0;
	int maxPower;

	PID pd;
};

}
