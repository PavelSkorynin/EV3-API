/*
 * MoveByEncoderOnArcProcess.hpp
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

class MoveByEncoderOnArcProcess: public virtual Process {
public:
	MoveByEncoderOnArcProcess(MotorPtr leftMotor, MotorPtr rightMotor, int leftEncoderDistance, int rightEncoderDistance, int maxPower = 70);

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted() const;

	/**
	 * Установить максимальную мощность двигателей
	 * @param maxPower максимальная мощность (от 0 до 100)
	 */
	void setMaxPower(int maxPower);

	/**
	 * Установить коэффициенты ПИД-регулятора
	 * @param kP пропорциональная составляющая
	 * @param kI интегральная составляющая
	 * @param kD дифференциальная составляющая
	 */
	void setPID(float kP, float kI, float kD);
protected:
	MotorPtr leftMotor;
	MotorPtr rightMotor;
	int leftEncoderDistance;
	int rightEncoderDistance;
	int maxPower;

	PID pd;
	bool toggleLeftMotor;
	bool toggleRightMotor;
};

}
