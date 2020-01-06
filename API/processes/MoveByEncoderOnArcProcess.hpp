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

class MoveByEncoderOnArcProcess: public Process {
public:
	MoveByEncoderOnArcProcess(MotorPtr leftMotor, MotorPtr rightMotor, int leftEncoderDistance, int rightEncoderDistance, int maxPower = 70);
	virtual ~MoveByEncoderOnArcProcess() = default;

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted() const;

	void setMaxPower(int maxPower);
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
