/*
 * StopOnLineProcess.h
 *
 *  Created on: 27 дек. 2019 г.
 *      Author: Pavel Skorynin
 */

#pragma once

#include <Process.h>
#include <Motor.h> // для моторов
#include <Sensor.h> // для датчиков
#include <PID.h>
#include "MoveOnLineProcess.hpp"

namespace ev3 {

class StopOnLineProcess: public Process {
public:
	StopOnLineProcess(MotorPtr leftMotor, MotorPtr rightMotor,
			SensorPtr leftLight, SensorPtr rightLight,
			int encoderDistance, int maxPower = 70);
	virtual ~StopOnLineProcess() = default;

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted() const;

protected:
	MotorPtr leftMotor;
	MotorPtr rightMotor;
	SensorPtr leftLight;
	SensorPtr rightLight;
	int encoderDistance;
	int maxPower;

	PID powerPD;
	std::shared_ptr<MoveOnLineProcess> moveOnLineProcess;
};

}
