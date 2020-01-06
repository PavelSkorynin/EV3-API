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

class MoveOnLineProcess: public Process {
public:
	MoveOnLineProcess(MotorPtr leftMotor, MotorPtr rightMotor,
			SensorPtr leftLight, SensorPtr rightLight,
			int encoderDistance, int maxPower = 70);
	virtual ~MoveOnLineProcess() = default;

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted() const;

	void setMaxPower(int maxPower);

protected:
	MotorPtr leftMotor;
	MotorPtr rightMotor;
	SensorPtr leftLight;
	SensorPtr rightLight;
	int encoderDistance;
	int maxPower;

	PID pd;
};

}
