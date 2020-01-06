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

class StopByEncoderOnArcProcess: public Process {
public:
	StopByEncoderOnArcProcess(MotorPtr leftMotor_, MotorPtr rightMotor_,
			int leftEncoderDistance_, int rightEncoderDistance_, int maxPower_);
	virtual ~StopByEncoderOnArcProcess() = default;

	virtual void update(float secondsFromStart) override;
	virtual void onStarted(float secondsFromStart) override;
	virtual void onCompleted(float secondsFromStart) override;
	virtual bool isCompleted() const;

protected:
	MotorPtr leftMotor;
	MotorPtr rightMotor;
	int leftEncoderDistance;
	int rightEncoderDistance;
	int maxPower;

	PID powerPD;
	std::shared_ptr<MoveByEncoderOnArcProcess> moveByEncoderOnArcProcess;
};

}
