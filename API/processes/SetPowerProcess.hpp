/*
 * SetPowerProcess.hpp
 *
 *  Created on: 20 февр. 2020 г.
 *      Author: Pavel
 */

#pragma once

#include <Process.h>
#include <Motor.h>

namespace ev3 {

class SetPowerProcess: public Process {
public:
	SetPowerProcess(MotorPtr motor, int power);
	virtual ~SetPowerProcess() = default;

	void onCompleted(float secondsFromStart) override;
	bool isCompleted(float secondsFromStart) override;

protected:
	MotorPtr motor;
	float power;
};

} /* namespace ev3 */
